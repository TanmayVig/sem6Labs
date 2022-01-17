//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// Embedded Systems Academy, Inc. assumes no responsibility or liability
// for the use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
// use without further testing or modification. 
//-----------------------------------------------------------------------------

#include "timer.h"
#include "LPC23xx.h" 

//*****************************************************// 
//  Public functions                                   //
//*****************************************************//

// initialize timer 0 to count milliseconds
void Timer_init(void) {
  // use timer 0 as timer (pclk = 12MHz)
  T0CTCR = 0x00;
  // prescaler - count at 1kHz
  T0PR = 12000-1;
  // no matches or interrupts
  T0MCR = 0x0000;
  // enable
  T0TCR = 0x01;
}

// returns timer 0 to same state as after reset
// needed so user applications can make assumptions about
// the state of the registers
void Timer_DeInit(void) {
  // disable
  T0TCR  = 0x00;
  T0CTCR = 0x00;
  T0PR   = 0;
  T0MCR  = 0x0000;
  T0TC   = 0;
  T0PC   = 0;
}

// get the current timer 0 count
unsigned long Timer_GetTimestamp(void) {
  return T0TC;
}

// check to see if a timestamp is in the past
// returns 1 if in the past, 0 if not
int Timer_HasTimestampExpired(unsigned long timestamp) {
  unsigned long now = T0TC;

  if (now > timestamp)
  {
    if ((now - timestamp) < 0x80000000)
      return 1;
    else
      return 0;
  }
  else
  {
    if ((timestamp - now) >= 0x80000000)
      return 1;
    else
      return 0;
  }
}

// pause for a specific number of milliseconds
void Timer_Delay(unsigned long milliseconds) {
  unsigned long timestamp = Timer_GetTimestamp() + milliseconds;
  while (!Timer_HasTimestampExpired(timestamp));
}
