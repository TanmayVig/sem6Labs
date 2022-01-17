//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// NXP Semiconductors assumes no responsibility or liability for the 
// use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
// use without further testing or modification. 
//-----------------------------------------------------------------------------

#include "LPC17XX.H"                        /* LPC23xx definitions */
#include "board_init.h"
#include "..\timer\timer.h"

void init_PLL(void)
{
  // disconnect PLL0
  LPC_SC->PLL0CON &= ~0x02;
  // feed
  LPC_SC->PLL0FEED = 0xAA;
  LPC_SC->PLL0FEED = 0x55;

  // disable PLL0
  LPC_SC->PLL0CON = 0x00;
  // feed
  LPC_SC->PLL0FEED = 0xAA;
  LPC_SC->PLL0FEED = 0x55;

  // enable main oscillator, 1MHz - 20MHz
  LPC_SC->SCS = 0x20;
  // wait until ready
  while (!(LPC_SC->SCS >> 6 & 0x1));
  // select main oscillator (10MHz) as input for PLL0
  LPC_SC->CLKSRCSEL = 0x01;

  // configure PLL0
  LPC_SC->PLL0CFG = (PLL_NSEL << 16) | PLL_MSEL;
  // feed
  LPC_SC->PLL0FEED = 0xAA;
  LPC_SC->PLL0FEED = 0x55;

  // enable PLL0
  LPC_SC->PLL0CON = 0x01;
  // feed
  LPC_SC->PLL0FEED = 0xAA;
  LPC_SC->PLL0FEED = 0x55;

  // set CPU clock divider - CCLK = 70MHz
  LPC_SC->CCLKCFG = CCLK_DIV;

  // wait for lock
  while (!((LPC_SC->PLL0STAT >> 26) & 0x1));

  // connect PLL0
  LPC_SC->PLL0CON = 0x03;
  // feed
  LPC_SC->PLL0FEED = 0xAA;
  LPC_SC->PLL0FEED = 0x55;

  // wait for connected
  while (!((LPC_SC->PLL0STAT >> 25) & 0x1));
}

void init_MAM(void)
{
}

void init_PCB(void)
{
}

void VectorsRemap(void)
{
}

void board_init(void)
{
  init_PLL();
  Timer_init();
	// wait for PLL to complete initialization after POR
	Timer_Delay(100);
	init_MAM();
	init_PCB();
	VectorsRemap();
}
