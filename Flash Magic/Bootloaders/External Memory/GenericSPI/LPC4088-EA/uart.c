// uart layer for LPC4088
// interfaces between CMSIS UART API and simplified UART API that
// the bootloader requires
// $Id: uart.c 3893 2015-05-12 11:46:44Z andy $

#include <stdint.h>
#include "lpc_pinsel.h"
#include "lpc_uart.h"
#include "host.h"

// initializes the uart for sending and receiving
void Host_Initialize
  (
  uint32_t baudrate                                      // baudrate in kbps
  )
{
  UART_CFG_Type UARTConfig;
  UART_FIFO_CFG_Type UARTRxFIFO;

  UART_DeInit(UART_0);
  
  // configure UART0
  UARTConfig.Baud_rate = baudrate;
  UARTConfig.Parity = UART_PARITY_NONE;
  UARTConfig.Databits = UART_DATABIT_8;
  UARTConfig.Stopbits = UART_STOPBIT_1;
  UART_Init(UART_0, &UARTConfig);
  // pinsel for UART0
  PINSEL_ConfigPin(0, 2, 1); // LPC4088 P0.2 as U2_TXD
  PINSEL_ConfigPin(0, 3, 1); // LPC4088 P0.3 as U2_RXD
  // enable transmit
  UART_TxCmd(UART_0, ENABLE);

  // create receive FIFO
  UART_FIFOConfigStructInit(&UARTRxFIFO);
  UART_FIFOConfig(UART_0, &UARTRxFIFO);
}

// sends data from the UART
void Host_SendData
  (
  uint8_t *data,
  int length
  )
{
  UART_Send(UART_0, data, length, BLOCKING);
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
  return UART_Receive(UART_0, buffer, maxlength, NONE_BLOCKING);
}
