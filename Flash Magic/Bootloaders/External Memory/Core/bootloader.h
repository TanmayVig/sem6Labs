// bootloader functionality
// supported by Flash Magic and allows programming of external
// memory devices
// (C) Embedded Systems Academy, Inc. 2012 All Rights Reserved
// May be used "as-is" without warranty, express or implied
// Use at your own risk
// $Id: bootloader.h 4460 2016-12-22 17:18:07Z ajayre $

#ifndef _BOOTLOADERH_
#define _BOOTLOADERH_

#include <stdint.h>

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

extern volatile uint16_t gBootloaderTick;

extern int Bootloader_Init(uint32_t clock);
extern void Bootloader_Process(void);
extern void Bootloader_SendError(void);

#endif // _BOOTLOADERH_
