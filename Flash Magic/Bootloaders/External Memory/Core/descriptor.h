// bootloader descriptor
// defines a block of data that describes the bootloader
// do not modify the contents of this file
// $Id: descriptor.h 5048 2018-05-03 15:45:03Z ajayre $


#ifndef _DESCRIPTORH_
#define _DESCRIPTORH_

#include <stdint.h>

// header and footer to mark start and end of structure in hex file
#define DESCRIPTOR_HEADER 0xAA557F3D
#define DESCRIPTOR_FOOTER 0x3D7F55AA

// version of the descriptor described in this file
#define DESCRIPTOR_VERSION 3

// maximum length of a bootloader name in characters including null terminator
#define DESCRIPTOR_MAXNAMELENGTH 64
// maxiumum length of a bootloader description in characters including null terminator
#define DESCRIPTOR_MAXDESCLENGTH 128
// maxiumum length of bootloader notes in characters including null terminator
#define DESCRIPTOR_MAXNOTESLENGTH 256

// bootstrap methods
#define DESCRIPTOR_BOOTSTRAP_BOOTROM     0
#define DESCRIPTOR_BOOTSTRAP_CORTEXUART  1
#define DESCRIPTOR_BOOTSTRAP_SWDLINK2    2
#define DESCRIPTOR_BOOTSTRAP_SWDLPC11U35 3
#define DESCRIPTOR_BOOTSTRAP_DAPLINK     4  // not yet supported

// bootloader functionality
// undefined bits are reserved and must be set to zero
#define BOOTLOADER_FUNC_BASIC (1UL << 0)                   // erase, erase sectors, program

// bootloader descriptor
// 32-bit field alignment
// values must be little-endian
typedef struct _descriptor
{
  uint32_t header;                                         // must be set to DESCRIPTOR_HEADER
  uint32_t descriptorversion;                              // must be set to DESCRIPTOR_VERSION
  uint32_t functionality;                                  // bootloader functionality (ORing of BOOTLOADER_FUNC_xxx)
  uint32_t bootloaderversion;                              // version number of bootloader
  uint32_t startaddress;                                   // programmable memory start address
  uint32_t endaddress;                                     // programmable memory end address
  uint32_t ramstartaddress;                                // ram start address (for bootloader storage)
  uint32_t ramendaddress;                                  // ram end address (for bootloader storage)
  uint32_t sectorsize;                                     // programmable sector size in bytes
  uint8_t  flasherasedvalue;                               // programmable memory erased value
  uint32_t programsize;                                    // number of bytes to program at once - cannot be larger than MAXPROGRAMSIZE
  uint32_t baudrate;                                       // baudrate to use for communication with bootloader
  uint32_t bootstrapmethod;                                // method of downloading bootloader (DESCRIPTOR_BOOTSTRAP_xxx)
  uint32_t bootstrapbaudrate;                              // baudeate to use for downloading of bootloader
  uint32_t ramcmdmailboxaddress;                           // ram address for commmand mailbox (0 = not used)
  uint32_t ramrespmailboxaddress;                          // ram address for response mailbox (0 = not used)
  uint32_t numrammailboxes;                                // number of ram mailboxes in each direction (0 = not used)
  char     devicename[DESCRIPTOR_MAXNAMELENGTH];           // name of device
  char     name[DESCRIPTOR_MAXNAMELENGTH];                 // name (DESCRIPTOR_MAXNAMELENGTH characters or shorter)
  char     description[DESCRIPTOR_MAXDESCLENGTH];          // description (DESCRIPTOR_MAXDESCLENGTH characters or shorter)
  char     notes[DESCRIPTOR_MAXNOTESLENGTH];               // notes (DESCRIPTOE_MAXNOTESLENGTH characters or shorter) - use semicolons for newlines
  uint32_t footer;                                         // must be set to DESCRIPTOR_FOOTER
} descriptor_t;

extern const descriptor_t Descriptor;

#endif // _DESCRIPTORH_
