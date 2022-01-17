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

#include "chip.h"
#include "timer.h"
#include "stopwatch.h"

//*****************************************************// 
//  Public functions                                   //
//*****************************************************//

// initialize timer 0 to count milliseconds
void Timer_init(void) {
  
  StopWatch_Init();
  StopWatch_Start();
}

// returns timer 0 to same state as after reset
// needed so user applications can make assumptions about
// the state of the registers
void Timer_DeInit(void) {
  // disable
  LPC_TIMER0->TCR &= ~TIMER_ENABLE;
}

// get the current timer value
unsigned long Timer_GetTimestamp(void) {
  
  return StopWatch_TicksToMs(StopWatch_Start());
}

// check to see if a timestamp is in the past
// returns 1 if in the past, 0 if not
int Timer_HasTimestampExpired(unsigned long timestamp) {
  unsigned long now = StopWatch_TicksToMs(StopWatch_Start());

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
