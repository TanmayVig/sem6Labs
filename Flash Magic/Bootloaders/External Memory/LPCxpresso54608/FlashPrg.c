// erasing, programming and verifying of SPIFI flash on
// LPCxpresso54608 evaluation board
// $Id: FlashPrg.c 4470 2017-01-13 11:18:15Z ajayre $

#include <stdint.h>
#include "descriptor.h"
#include "board.h"
#include "string.h"
#include "fsl_spifi.h"
#include "board.h"
#include "pin_mux.h"
#include "host.h"
#include <stdbool.h>
#include <stdio.h>

// bootloader version
#define VERSION 2

#define START_ADDRESS 0x10000000
#define END_ADDRESS   0x107FFFFF
#define PAGE_SIZE (256)
#define SECTOR_SIZE (0x10000)

// bootloader descriptor
// values must be little-endian
const descriptor_t Descriptor =
{
  DESCRIPTOR_HEADER,                           // must be DESCRIPTOR_HEADER
  DESCRIPTOR_VERSION,                          // must be DESCRIPTOR_VERSION
  BOOTLOADER_FUNC_BASIC,                       // bootloader functionality (ORing of BOOTLOADER_FUNC_xxx)
  VERSION,                                     // version number of bootloader
  START_ADDRESS,                               // programmable memory start address
  END_ADDRESS,                                 // programmable memory end address
  0x20000000,                                  // ram start address (for bootloader storage)
  0x20008FFF,                                  // ram end address (for bootloader storage)
  SECTOR_SIZE,                                 // programmable sector size in bytes
  0xFF,                                        // programmable memory erased value
  PAGE_SIZE,                                   // number of bytes to program at once
  0,                                           // communication baudrate
  DESCRIPTOR_BOOTSTRAP_SWDLINK2,               // use SWD to download this bootloader
  0,                                           // baudrate to use for downloading bootloader
  0x20010000,                                  // ram address for commmand mailbox (0 = not used)
  0x20011000,                                  // ram address for response mailbox (0 = not used)
  16,                                          // number of ram mailboxes in each direction (0 = not used)
  "LPC54608J512",                              // name of device
  "LPCxpresso54608",                           // name (DESCRIPTOR_MAXNAMELENGTH characters or shorter)
  "SPIFI Flash",                               // description (DESCRIPTOR_MAXDESCLENGTH characters or shorter)
  "Make DFULink jumper, connect to J5",        // notes (DESCRIPTOR_MAXNOTESLENGTH characters or shorter) - use semicolons for newlines
  DESCRIPTOR_FOOTER                            // must be DESCRIPTOR_FOOTER
};

// memory access macros
#define M8(adr)  (*((volatile unsigned char  *) (adr)))
#define M16(adr) (*((volatile unsigned short *) (adr)))
#define M32(adr) (*((volatile unsigned long  *) (adr)))

// commands
#define COMMAND_WRITE        0x02
#define COMMAND_READ         0x03
#define COMMAND_WRITEDISABLE 0x04
#define COMMAND_READSTATUS   0x05
#define COMMAND_WRITEENABLE  0x06
#define COMMAND_CHIPERASE    0x60
#define COMMAND_READID       0x9F
#define COMMAND_BLOCKERASE   0xD8

////////////////////////////////////////////////////////////////////////////
// MODULE FUNCTIONS

#define EXAMPLE_SPIFI SPIFI0
#define EXAMPLE_SPI_BAUDRATE (12000000)
#define COMMAND_NUM (6)
#define READ (0)
#define PROGRAM_PAGE (1)
#define GET_STATUS (2)
#define ERASE_SECTOR (3)
#define WRITE_ENABLE (4)
#define WRITE_REGISTER (5)

uint8_t g_buffer[PAGE_SIZE] = {0};

spifi_command_t command[COMMAND_NUM] = {
    {PAGE_SIZE, false, kSPIFI_DataInput, 1, kSPIFI_CommandDataQuad, kSPIFI_CommandOpcodeAddrThreeBytes, 0x6B},
    {PAGE_SIZE, false, kSPIFI_DataOutput, 0, kSPIFI_CommandOpcodeSerial, kSPIFI_CommandOpcodeAddrThreeBytes, 0x38},
    {4, false, kSPIFI_DataInput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x05},
    {0, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeAddrThreeBytes, 0xD8},
    {0, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x06},
    {4, false, kSPIFI_DataOutput, 0, kSPIFI_CommandAllSerial, kSPIFI_CommandOpcodeOnly, 0x01}};

void check_if_finish()
{
    uint32_t val = 0;
    /* Check WIP bit */
    do
    {
        SPIFI_SetCommand(EXAMPLE_SPIFI, &command[GET_STATUS]);
        while ((EXAMPLE_SPIFI->STAT & SPIFI_STAT_INTRQ_MASK) == 0U)
        {
        }
        val = SPIFI_ReadData(EXAMPLE_SPIFI);
    } while (val & 0x1);
}

void enable_quad_mode()
{
    /* Write enable */
    SPIFI_SetCommand(EXAMPLE_SPIFI, &command[WRITE_ENABLE]);

    /* Set write register command */
    SPIFI_SetCommand(EXAMPLE_SPIFI, &command[WRITE_REGISTER]);

    SPIFI_WriteData(EXAMPLE_SPIFI, 0x40);

    check_if_finish();
}

////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS

// initialize module and enable access to memory to be programmed
// returns 0 for OK, 1 for failed
int Memory_Init
  (
  unsigned long adr,                                       // base address of memory
  unsigned long clk                                        // cpu clock frequency in MHz
  ) 
{
  uint32_t sourceClockFreq;

  spifi_config_t config = {0};

  // attach 12 MHz clock to SPI3
  CLOCK_AttachClk(kFRO12M_to_FLEXCOMM9);

  // Set SPIFI clock source
  CLOCK_AttachClk(kMAIN_CLK_to_SPIFI_CLK);
  sourceClockFreq = CLOCK_GetSpifiClkFreq();

  // Set the clock divider
  CLOCK_SetClkDiv(kCLOCK_DivSpifiClk, sourceClockFreq / EXAMPLE_SPI_BAUDRATE - 1U, false);

  // Initialize SPIFI
  SPIFI_GetDefaultConfig(&config);
  SPIFI_Init(EXAMPLE_SPIFI, &config);

  // Enable Quad mode
  enable_quad_mode();

  // Setup memory command
  SPIFI_SetMemoryCommand(EXAMPLE_SPIFI, &command[READ]);

  // Reset the SPIFI to switch to command mode
  SPIFI_ResetCommand(EXAMPLE_SPIFI);
  EnableIRQ(SPIFI0_IRQn);

  return 0;
}


// erases a sector of memory
// returns 0 for OK, 1 for failed
int Memory_EraseSector
  (
  unsigned long adr                                        // address of sector to erase
  ) 
{
  // Reset to memory command mode
  SPIFI_ResetCommand(EXAMPLE_SPIFI);

  // Write enable
  SPIFI_SetCommand(EXAMPLE_SPIFI, &command[WRITE_ENABLE]);
  // Set address
  SPIFI_SetCommandAddress(EXAMPLE_SPIFI, adr);
  // Erase sector
  SPIFI_SetCommand(EXAMPLE_SPIFI, &command[ERASE_SECTOR]);
  // Check if finished
  check_if_finish();

  return 0;
}


// erase entire memory
// returns 0 for OK, 1 for failed
int Memory_EraseChip(void) 
{
  uint32_t addr;
  
  for (addr = START_ADDRESS; addr < END_ADDRESS; addr += SECTOR_SIZE)
  {
    Memory_EraseSector(addr);
  }
  
  return 0;
}


// programs a page in memory
// returns 0 for OK, 1 for failed
int Memory_ProgramPage
  (
  unsigned long adr,                                       // address of page
  unsigned long sz,                                        // size of page
  unsigned char *buf                                       // data to program into page
  ) 
{
  int i, j;
  uint32_t data;
  
  // Reset to memory command mode
  SPIFI_ResetCommand(EXAMPLE_SPIFI);

  SPIFI_SetCommand(EXAMPLE_SPIFI, &command[WRITE_ENABLE]);
  SPIFI_SetCommandAddress(EXAMPLE_SPIFI, adr);
  SPIFI_SetCommand(EXAMPLE_SPIFI, &command[PROGRAM_PAGE]);

  for (i = 0; i < PAGE_SIZE; i += 4)
  {
      for (j = 0; j < 4; j++)
      {
          data |= ((uint32_t)(buf[i + j])) << (j * 8);
      }
      SPIFI_WriteData(EXAMPLE_SPIFI, data);
      data = 0;
  }
  check_if_finish();

  return 0;
}


// reads a byte from memory
// returns the read byte
unsigned char Memory_ReadByte
  (
  unsigned long adr                                        // address of location to read
  )
{
  /* Reset to memory command mode */
  SPIFI_ResetCommand(EXAMPLE_SPIFI);

  SPIFI_SetMemoryCommand(EXAMPLE_SPIFI, &command[READ]);

  return *(uint8_t *)adr;
}
