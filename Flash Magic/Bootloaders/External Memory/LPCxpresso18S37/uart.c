// uart layer for LPC1850
// interfaces between CMSIS UART API and simplified UART API that
// the bootloader requires
// $Id: uart.c 3893 2015-05-12 11:46:44Z andy $

#include <stdint.h>
#include "lpc18xx_uart.h"
#include "lpc18xx_scu.h"
#include "host.h"

// initializes the uart for sending and receiving
void Host_Initialize
  (
  uint32_t baudrate                                      // baudrate in kbps
  )
{
  // bootloader has configured UART0 to use IRC at 12MHz, 8N1
  // we just need to change the baudrate

  LPC_USART0->LCR |= (1UL << 7);

  switch (baudrate)
  {
    case 19200:
      // DL = 24, FDR = 0.625
      // 12MHz / (16 x 24 x (1 + 0.625) = 19230
      LPC_USART0->DLL = 24;
      LPC_USART0->DLM = 0;
      LPC_USART0->FDR = (8 << 4) + 5;  // 5/8 = 0.625
      break;

    case 57600:
      // DL = 8, FDR = 0.625
      // 12MHz / (16 x 8 x (1 + 0.625) = 57692
      LPC_USART0->DLL = 8;
      LPC_USART0->DLM = 0;
      LPC_USART0->FDR = (8 << 4) + 5;  // 5/8 = 0.625
      break;
    
    case 115200:
      // DL = 4, FDR = 0.625
      // 12MHz / (16 x 4 x (1 + 0.625) = 115384
      LPC_USART0->DLL = 4;
      LPC_USART0->DLM = 0;
      LPC_USART0->FDR = (8 << 4) + 5;  // 5/8 = 0.625
      break;
  }

  LPC_USART0->LCR &= ~(1UL << 7);
}

// sends data from the UART
void Host_SendData
  (
  uint8_t *data,
  int length
  )
{
  UART_Send(LPC_USART0, data, length, BLOCKING);
}

// receives data from the UART and stores it in a buffer
// returns number of bytes received or zero for none
// should return immediately if there is no data to be read
int Host_ReceiveData
  (
  uint8_t *buffer,
  int maxlength
  )
{
  return UART_Receive(LPC_USART0, buffer, maxlength, NONE_BLOCKING);
}
