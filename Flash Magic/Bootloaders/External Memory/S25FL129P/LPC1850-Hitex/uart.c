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
  UART_CFG_Type UARTConfig;
  UART_FIFO_CFG_Type UARTRxFIFO;

  // configure USART3
  UARTConfig.Baud_rate = baudrate;
  UARTConfig.Parity = UART_PARITY_NONE;
  UARTConfig.Databits = UART_DATABIT_8;
  UARTConfig.Stopbits = UART_STOPBIT_1;
  UART_Init(LPC_USART3, &UARTConfig);
  // use P2.3 and P2.4 for USART3
  scu_pinmux(2, 3, MD_PDN, FUNC2);                       // TXD
  scu_pinmux(2, 4, MD_PLN | MD_EZI | MD_ZI, FUNC2);      // RXD
  // enable transmit
  UART_TxCmd(LPC_USART3, ENABLE);

  // create receive FIFO
  UART_FIFOConfigStructInit(&UARTRxFIFO);
  UART_FIFOConfig(LPC_USART3, &UARTRxFIFO);
}

// sends data from the UART
void Host_SendData
  (
  uint8_t *data,
  int length
  )
{
  UART_Send(LPC_USART3, data, length, BLOCKING);
}

// sends a debug message to the host
// message must be null terminated
void Host_Debug(char *Message)
{
  // not supported
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
  return UART_Receive(LPC_USART3, buffer, maxlength, NONE_BLOCKING);
}
