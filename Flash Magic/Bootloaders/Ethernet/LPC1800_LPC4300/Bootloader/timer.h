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

#ifndef __timer_H
#define __timer_H

void Timer_init(void);
void Timer_DeInit(void);
unsigned long Timer_GetTimestamp(void);
int Timer_HasTimestampExpired(unsigned long timestamp);
void Timer_Delay(unsigned long milliseconds);

#endif
