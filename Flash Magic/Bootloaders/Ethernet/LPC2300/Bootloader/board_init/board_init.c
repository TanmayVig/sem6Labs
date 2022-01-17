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

#include <LPC23XX.H>                        /* LPC23xx definitions */
#include "board_init.h"
#include "..\timer\timer.h"

void init_PLL(void)
{
    unsigned MValue, NValue;
  
    if ( PLLSTAT & (1 << 25) )
    {
  	PLLCON = 1;			/* Enable PLL, disconnected */
  	PLLFEED = 0xaa;
  	PLLFEED = 0x55;
    }
  
    PLLCON = 0;				/* Disable PLL, disconnected */
    PLLFEED = 0xaa;
    PLLFEED = 0x55;
      
    SCS |= 0x20;			/* Enable main OSC */
    while( !(SCS & 0x40) );	/* Wait until main OSC is usable */
  
    CLKSRCSEL = 0x1;		/* select main OSC, 12MHz, as the PLL clock source */
  
    PLLCFG = PLL_MVAL | (PLL_NVAL << 16);
    PLLFEED = 0xaa;
    PLLFEED = 0x55;
        
    PLLCON = 1;				/* Enable PLL, disconnected */
    PLLFEED = 0xaa;
    PLLFEED = 0x55;
  
    CCLKCFG = CCLKDIV;	/* Set clock divider */
  
    while ( ((PLLSTAT & (1 << 26)) == 0) );	/* Check lock bit status */
      
    MValue = PLLSTAT & 0x00007FFF;
    NValue = (PLLSTAT & 0x00FF0000) >> 16;
    while ((MValue != PLL_MVAL) && ( NValue != PLL_NVAL) );
  
    PLLCON = 3;				/* enable and connect */
    PLLFEED = 0xaa;
    PLLFEED = 0x55;
    while ( ((PLLSTAT & (1 << 25)) == 0) );	/* Check connect bit status */
    return;
}

void init_MAM(void)
{
    MAMCR = 0;
    MAMTIM = 3;
    MAMCR = 2;
}

void init_PCB(void)
{
  /* Turn Off all LEDs  */
  FIO2DIR |= 0x0FF;
  FIO2CLR |= 0x0FF;
}

void VectorsRemap(void)
{
}

void board_init(void)
{
    Timer_init();
    init_PLL();
	// wait for PLL to complete initialization after POR
	Timer_Delay(100);
	init_MAM();
	init_PCB();
	VectorsRemap();
}
