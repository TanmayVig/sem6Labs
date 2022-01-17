// blinky demonstration for Keil MCB1700
// flashes the LED on P1.28
// Embedded Systems Academy, Inc.
// www.esacademy.com

#include "LPC17xx.h"

// PLL configuration, M = 64, N = 6
// input = 12MHz, Fcco = (2 x M x 12MHz) / N = 256MHz
// divded by 4 for CCLK of 64MHz
#define PLL_MSEL (64 - 1)
#define PLL_NSEL (6 - 1)
#define CCLK_DIV (4 - 1)

int main(void)
{
  int on = 0;
  volatile int counter = 0;

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

  // no pull up/down on P1.28
  LPC_PINCON->PINMODE3 = (0x02UL << 24);
  // P1.28 is output
  LPC_GPIO1->FIODIR |= 1 << 28;

  // toggle led
  while(1)
  {
    if (!on)
    {
      LPC_GPIO1->FIOSET = 1 << 28;
      on = 1;
      for (counter = 0; counter < 5000000; counter++);
    }
    else
    {
      LPC_GPIO1->FIOCLR = 1 << 28;
      on = 0;
      for (counter = 0; counter < 5000000; counter++);
    }
  }
}
