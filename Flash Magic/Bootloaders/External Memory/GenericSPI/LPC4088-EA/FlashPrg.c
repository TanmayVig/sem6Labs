// erasing, programming and verifying of SPIFI flash on
// Embedded Artists LPC4088 evaluation board
// Adapted from code in NXP SPIFI Read/Write example
// see: http://sw.lpcware.com
// $Id: FlashPrg.c 3893 2015-05-12 11:46:44Z andy $

#include <stdint.h>             
#include "descriptor.h"
#include "lpc_spifi_rom_api.h"
#include "lpc407x_8x_177x_8x.h"

// bootloader version
#define VERSION 1

// bootloader descriptor
// values must be little-endian
const descriptor_t Descriptor =
{
  DESCRIPTOR_HEADER,                                                          // must be DESCRIPTOR_HEADER
  DESCRIPTOR_VERSION,                                                         // must be DESCRIPTOR_VERSION
  BOOTLOADER_FUNC_BASIC,                                                      // bootloader functionality (ORing of BOOTLOADER_FUNC_xxx)
  VERSION,                                                                    // version number of bootloader
  0x28000000,                                                                 // programmable memory start address
  0x283FFFFF,                                                                 // programmable memory end address
  0x10000500,                                                                 // ram start address (for bootloader storage)
  0x1000FFFF,                                                                 // ram end address (for bootloader storage)
  0x40000,                                                                    // programmable sector size in bytes
  0xFF,                                                                       // programmable memory erased value
  256,                                                                        // number of bytes to program at once
  115200,                                                                     // communication baudrate
  DESCRIPTOR_BOOTSTRAP_CORTEXUART,                                            // use ISP bootloader feature to download this bootloader
  57600,                                                                      // baudrate to use for downloading bootloader
  0,                                                                          // ram address for commmand mailbox (0 = not used)
  0,                                                                          // ram address for response mailbox (0 = not used)
  0,                                                                          // number of ram mailboxes in each direction (0 = not used)
  "LPC4088",                                                                  // name of device
  "Generic SPI LPC4088 EA",                                                   // name (DESCRIPTOR_MAXNAMELENGTH characters or shorter)
  "NXP SPIFI library on EA eval board",                                       // description (DESCRIPTOR_MAXDESCLENGTH characters or shorter)
  "Should work with any SPIFI device compatible with the NXP SPIFI library;;" // notes (DESCRIPTOR_MAXNOTESLENGTH characters or shorter) - use semicolons for newlines
  "For the EA LPC4088 board:;;- Ensure JP20 is connected;- Set T1 and T2 to"
  "400ms;- Connect Flash Magic to J25 for ISP",
  DESCRIPTOR_FOOTER                                                           // must be DESCRIPTOR_FOOTER
};

// memory access macros
#define M8(adr)  (*((volatile unsigned char  *) (adr)))
#define M16(adr) (*((volatile unsigned short *) (adr)))
#define M32(adr) (*((volatile unsigned long  *) (adr)))

// module-level variables
static SPIFIobj obj;
static SPIFI_RTNS * pSpifi;
static SPIFIopers opers;


// initialize module and enable access to memory to be programmed
// returns 0 for OK, 1 for failed
int Memory_Init
  (
  unsigned long adr,                                       // base address of memory
  unsigned long clk                                        // cpu clock frequency in Hz
  ) 
{	
  // turn power on to SPIFI
  LPC_SC->PCONP |= 0x00010000;
  
  LPC_IOCON->P2_7  &= ~0x07;
  LPC_IOCON->P2_7  |= 0x05;    // SPIFI_CSN @ P2.7
  LPC_IOCON->P0_22 &= ~0x07;
  LPC_IOCON->P0_22 |= 0x05;    // SPIFI_CLK @ P0.22
  LPC_IOCON->P0_15 &= ~0x07;
  LPC_IOCON->P0_15 |= 0x05;    // SPIFI_IO2 @ P0.15
  LPC_IOCON->P0_16 &= ~0x07;
  LPC_IOCON->P0_16 |= 0x05;    // SPIFI_IO3 @ P0.16
  LPC_IOCON->P0_17 &= ~0x07;
  LPC_IOCON->P0_17 |= 0x05;    // SPIFI_IO1 @ P0.17
  LPC_IOCON->P0_18 &= ~0x07;
  LPC_IOCON->P0_18 |= 0x05;    // SPIFI_IO0 @ P0.18
  
  pSpifi = &spifi_table;

	return pSpifi->spifi_init(&obj, 4, S_RCVCLK | S_FULLCLK, 60);
}


// erase entire memory
// returns 0 for OK, 1 for failed
int Memory_EraseChip(void) 
{
  unsigned long i;
  
  for (i = 0; i < obj.memSize / 4; i += 4)
  {
    if (*((unsigned long *)(obj.base+i)) != 0xFFFFFFFF)
    {
      opers.dest = (char *)(obj.base);
      opers.length = obj.memSize;
      opers.scratch = NULL;
      opers.options = S_VERIFY_ERASE;
      if (pSpifi->spifi_erase(&obj, &opers)) return 1;
    }
  }

  return 0;
}


// erases a sector of memory
// returns 0 for OK, 1 for failed
int Memory_EraseSector
  (
  unsigned long adr                                        // address of sector to erase
  ) 
{
  opers.dest = (char *)adr;
	if(obj.devSize < 1024*256)
		opers.length = obj.devSize;
	else
		opers.length = 1024*256;
  opers.scratch = NULL;
  opers.options = S_VERIFY_ERASE;
  
  return pSpifi->spifi_erase(&obj, &opers);
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
	opers.dest = (char *)adr;
	opers.length = sz;
	opers.scratch = NULL;
	opers.protect = 0;
	opers.options = S_CALLER_ERASE;
	return pSpifi->spifi_program(&obj, (char *)buf, &opers);
}


// reads a byte from memory
// returns the read byte
unsigned char Memory_ReadByte
  (
  unsigned long adr                                        // address of location to read
  )
{
  return *((volatile unsigned char  *)adr);
}
