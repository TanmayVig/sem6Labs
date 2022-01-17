// Debug output
// $Id: debug.c 4467 2017-01-12 13:34:59Z ajayre $

#include "debug.h"
#include "fsl_usart.h"
#include <stdio.h>
#include <stdarg.h>

#define MAX_DEBUG_LINE 128
static char Line[MAX_DEBUG_LINE];

// sends a debug message
int Debug_printf
  (
  char *Format,
  ...
  )
{
  va_list Args;
  int Length;
  
  va_start(Args, Format);
  Length = vsprintf(Line, Format, Args);
  va_end(Args);
  
  USART_WriteBlocking(USART, (const uint8_t *)Line, Length);
  
  return Length;
}
