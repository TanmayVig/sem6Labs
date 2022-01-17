// erasing, programming and verifying of SPIFI flash S25FL129P on
// Hitex LPC1800/LPC4300 evaluation board
// Adapted from code in NXP SPIFI Read/Write example
// see: http://sw.lpcware.com/?p=lpc18xx.git&a=tree
// $Id: FlashPrg.c 3895 2015-05-14 13:06:10Z andy $

#include <stdint.h>
#include "lpc18xx_scu.h"
#include "lpc18xx_cgu.h"             
#include "descriptor.h"
#include "spifi_rom_api.h"

// bootloader version
#define VERSION 1

// bootloader descriptor
// values must be little-endian
const descriptor_t Descriptor =
{
  DESCRIPTOR_HEADER,                           // must be DESCRIPTOR_HEADER
  DESCRIPTOR_VERSION,                          // must be DESCRIPTOR_VERSION
  BOOTLOADER_FUNC_BASIC,                       // bootloader functionality (ORing of BOOTLOADER_FUNC_xxx)
  VERSION,                                     // version number of bootloader
  0x80000000,                                  // programmable memory start address
  0x800FFFFF,                                  // programmable memory end address
  0x10000500,                                  // ram start address (for bootloader storage)
  0x10007FFF,                                  // ram end address (for bootloader storage)
  0x10000,                                     // programmable sector size in bytes
  0xFF,                                        // programmable memory erased value
  64,                                          // number of bytes to program at once
  0,                                           // communication baudrate
  DESCRIPTOR_BOOTSTRAP_SWDLINK2,               // use SWD to download this bootloader
  0,                                           // baudrate to use for downloading bootloader
  0x10080000,                                  // ram address for commmand mailbox (0 = not used)
  0x10081000,                                  // ram address for response mailbox (0 = not used)
  16,                                          // number of ram mailboxes in each direction (0 = not used)
  "LPC4337",                                   // name of device
  "LPCxpresso4337",                            // name (DESCRIPTOR_MAXNAMELENGTH characters or shorter)
  "MX25L8035E on SPIFI",                       // description (DESCRIPTOR_MAXDESCLENGTH characters or shorter)
  "Make DFULink jumper, connect to J5",        // notes (DESCRIPTOE_MAXNOTESLENGTH characters or shorter) - use semicolons for newlines
  DESCRIPTOR_FOOTER                            // must be DESCRIPTOR_FOOTER
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
	/* set SPIFI clock */
	CGU_EntityConnect(CGU_CLKSRC_IRC, CGU_BASE_SPIFI); /* IRC 12 MHz is good enough for us */

	/* set up SPIFI I/O (undocumented bit 7 set as 1, Aug 2 2011) */
	LPC_SCU->SFSP3_3 = 0xF3; /* high drive for SCLK */
	/* IO pins */
	LPC_SCU->SFSP3_4 = 0xD3;
	LPC_SCU->SFSP3_5 = 0xD3;
	LPC_SCU->SFSP3_6 = 0xD3;
	LPC_SCU->SFSP3_7 = 0xD3;
	LPC_SCU->SFSP3_8 = 0x13; /* CS doesn't need feedback */

  pSpifi = &spifi_table;

	return pSpifi->spifi_init(&obj, 3, S_RCVCLK | S_FULLCLK, 12);
}


// erase entire memory
// returns 0 for OK, 1 for failed
int Memory_EraseChip(void) 
{
  opers.dest = (char *)(obj.base);
  opers.length = obj.memSize;
  opers.scratch = NULL;
  opers.options = S_VERIFY_ERASE;
  
  return pSpifi->spifi_erase(&obj, &opers);
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
