// bootloader functionality
// supported by Flash Magic and allows programming of external
// memory devices
// (C) Embedded Systems Academy, Inc. 2012 All Rights Reserved
// May be used "as-is" without warranty, express or implied
// Use at your own risk
// $Id: bootloader.c 4467 2017-01-12 13:34:59Z ajayre $

#include "bootloader.h"
#include "descriptor.h"
#include "host.h"

// programming API
extern int Memory_Init(unsigned long adr, unsigned long clk);
extern int Memory_EraseChip(void);
extern int Memory_EraseSector(unsigned long adr);
extern int Memory_ProgramPage(unsigned long adr, unsigned long sz, unsigned char *buf);
extern unsigned char Memory_ReadByte(unsigned long adr);

// global tick counter - must be incremented every millisecond
volatile uint16_t gBootloaderTick = 0;

// uart receive buffer size
#define RXBUFFERSIZE 300

// maximum number of bytes of data to receive at once
// the descriptor programsize field must be same or smaller than this value
#define MAXPROGRAMSIZE 1024

// if no data received for this time then receive state machine is reset, in milliseconds
#define RECEIVETIMEOUT 1000

// states
#define STATE_HEADER 0
#define STATE_DATA   1

// commands
#define COMMAND_PROGRAM     0
#define COMMAND_ERASEALL    1
#define COMMAND_ERASESECTOR 2

// module variables
static uint8_t rxbuffer[RXBUFFERSIZE];
static uint8_t chunk[MAXPROGRAMSIZE];
static int state;
static uint32_t headerbytes;
static uint16_t lastbytetimestamp = 0;
static uint32_t databytes, chunkbytes;
static uint32_t chunkaddress;
static uint32_t localchecksum;
static uint32_t programstartaddr;
static uint8_t header[16];
static uint32_t datasize;
static uint32_t remotechecksum;

// module functions
static uint16_t GetTime(void);
static uint8_t IsTimeExpired(uint16_t timestamp);
static int VerifyChecksum(uint32_t startaddress, uint32_t endaddress, uint32_t checksum);

// initializes the bootloader
// returns 1 for success, 0 for error
int Bootloader_Init
  (
  uint32_t clock                                           // CPU clock frequency in MHz
  )
{
  // init host
  Host_Initialize(Descriptor.baudrate);

  // init command state machine
  state = STATE_HEADER;
  headerbytes = 0;

  if (Memory_Init(Descriptor.startaddress, clock))
  {
    // failed to init
    Host_SendData("INITERR\r\n", 9);
    return 0;
  }

  // send boot message
  Host_SendData("ESAFLASHOS\r\n", 12);
  
  return 1;
}

// executes the bootloader state machine
// call continually in a background loop or an RTOS task
void Bootloader_Process(void)
{
  uint32_t length, b;
  uint32_t sectoraddr;
  
  length = Host_ReceiveData(rxbuffer, RXBUFFERSIZE);
  if (length)
  {      
    // remember when data was received
    lastbytetimestamp = GetTime();

    for (b = 0; b < length; b++)
    {
      switch (state)
      {
        case STATE_HEADER:
          // autobaud character - tell the client that we are executing
          if ((rxbuffer[b] == '?') && !headerbytes)
          {
            Host_SendData("ESAFLASHOS\r\n", 12);
            break;
          }

          header[headerbytes++] = rxbuffer[b];
          // if received complete header then parse it...
          if (headerbytes == 16)
          {
            // first byte is the command
            switch (header[0])
            {
              case COMMAND_PROGRAM:
                // get size of data that is going to be sent
                datasize = ((uint32_t)header[2] << 8) | header[1];
                datasize *= Descriptor.programsize;
            
                // get checksum
                remotechecksum = ((uint32_t)header[6] << 24) | ((uint32_t)header[5] << 16) | ((uint32_t)header[4] << 8) | header[3];
            
                // get programming start address
                programstartaddr = ((uint32_t)header[10] << 24) | ((uint32_t)header[9] << 16) | ((uint32_t)header[8] << 8) | header[7];
              
                // now get data 
                state = STATE_DATA;
                databytes = 0;
                chunkbytes = 0;
                localchecksum = 0;
                chunkaddress = programstartaddr;
                break;

              case COMMAND_ERASEALL:
                // erase the flash memory
                if (Memory_EraseChip())
                {
                  Host_SendData("ERROR\r\n", 7);
                }
                else
                {
                  Host_SendData("OK\r\n", 4);
                }

                // wait for next header
                headerbytes = 0;
                break;

              case COMMAND_ERASESECTOR:
                // get sector address
                sectoraddr = ((uint32_t)header[4] << 24) | ((uint32_t)header[3] << 16) | ((uint32_t)header[2] << 8) | header[1];
                // erase sector
                if (Memory_EraseSector(sectoraddr))
                {
                  Host_SendData("ERROR\r\n", 7);
                }
                else
                {
                  Host_SendData("OK\r\n", 4);
                }

                // wait for next header
                headerbytes = 0;
                break;
                
              // unknown command
              default:
                // wait for next header
                headerbytes = 0;
                break;
            }
          }
          break;

        case STATE_DATA:
          // update checksum calculation
          localchecksum += rxbuffer[b];

          chunk[chunkbytes++] = rxbuffer[b];
          if (chunkbytes == Descriptor.programsize)
          {
            
            // program page
            if (!Memory_ProgramPage(chunkaddress, chunkbytes, chunk))
            {
              Host_SendData("PROGOK\r\n", 8);
              chunkaddress += chunkbytes;
              chunkbytes = 0;
            }
            else
            {
              Host_SendData("PROGERR\r\n", 9);
              // wait for next header
              state = STATE_HEADER;
              headerbytes = 0;
            }
          }

          databytes++;
          if (databytes == datasize)
          {
            // end of data transfer
            // check checksum
            localchecksum = 0xFFFFFFFF - localchecksum + 1;
                        
            if (localchecksum != remotechecksum)
            {
              Host_SendData("CHECKSUM\r\n", 10);
            }
            // verify flash
            else if (!VerifyChecksum(programstartaddr, chunkaddress - 1, localchecksum))
            {
              Host_SendData("VERIFY\r\n", 8);
            }
            else
            {
              Host_SendData("OK\r\n", 4);
            }
            // wait for next header
            state = STATE_HEADER;
            headerbytes = 0;
          }
          break;
      }
    }
  }
  else
  {
    // haven't received data in a while so reset state machine
    if (IsTimeExpired(lastbytetimestamp + RECEIVETIMEOUT))
    {
      // wait for next header
      state = STATE_HEADER;
      headerbytes = 0;
    }
  }
}

// verifies the contents of memory matches the checksum
// returns 1 if matches, 0 if does not match
static int VerifyChecksum(uint32_t startaddress, uint32_t endaddress, uint32_t checksum)
{
  uint32_t address;
  uint32_t chk = 0x00000000;

  // calculate checksum
  for (address = startaddress; address <= endaddress; address++)
  {
    chk += Memory_ReadByte(address); 
  }

  // 2's complement
  chk = 0xFFFFFFFF - chk + 1;

  // compare
  if (chk != checksum) return 0;

  return 1;
}


// reads the 1 millisecond timer count
// returns the current timer count
static uint16_t GetTime(void)
{
  return gBootloaderTick;
}


// compares a timer count and determines if the time has passed or not
// returns 1 if the timer count is in the past, returns 0 if the timer
// count is in the future
// note that the maximum time measurable is 0x8000 (about 32 seconds)
static uint8_t IsTimeExpired(uint16_t timestamp)
{
  uint16_t time_now;

  time_now = gBootloaderTick;
  if (time_now >= timestamp)
  {
    if ((time_now - timestamp) < 0x8000)
      return 1;
    else
      return 0;
  }
  else
  {
    if ((timestamp - time_now) >= 0x8000)
      return 1;
    else
      return 0;
  }
}
