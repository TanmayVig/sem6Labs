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

#include "uart.h"
#include "LPC17XX.H" 
#include "..\sbl_config.h"
#include "..\board_init\board_init.h"

#if ETHERNET_DEBUG
#define CR     0x0D

//*****************************************************// 
//  Private functions                                  //
//*****************************************************//
char conv(char c) {
	if (c<10)
	  c+=0x30;
    else
	  c+=0x37;
	return c;
}

//*****************************************************// 
//  Public functions                                   //
//*****************************************************//
void UART_init(unsigned int baud) {
  unsigned int divisor = (64000000 / 16) / baud;

  // uart0 is clocked from cclk/1
  LPC_SC->PCLKSEL0 &= ~(0x3UL << 6);
  LPC_SC->PCLKSEL0 |=  (0x1UL << 6);

  LPC_UART0->LCR = 0x83;                   // 8 bit, 1 stop bit, no parity, enable DLAB 
  LPC_UART0->DLL = divisor & 0xFF;
  LPC_UART0->DLM = (divisor >> 8) & 0xFF;
  LPC_UART0->LCR &= ~0x80;                 // Disable DLAB 
  LPC_UART0->FCR = 1;

  LPC_PINCON->PINSEL0 |= 0x00000050;           // RxD0 and TxD0 
}

int getchar (void) {
 				
  if (LPC_UART0->LSR & 0x01)
    return (LPC_UART0->RBR);
  else
    return (-1);
}

int putchar(int ch) {

  if (ch == '\n')  {
    while (!(LPC_UART0->LSR & 0x20));
    LPC_UART0->THR = CR;
  }
  while (!(LPC_UART0->LSR & 0x20));
  return (LPC_UART0->THR = ch);
 }

void print(char * str) {
  int i=0;

  while(1) {
	putchar(str[i]);
	if (str[i]=='\0')
	  break;
	i++;
  }
}

void printhexa(char num){
 
  putchar(conv((num>>4)&0x0F));
  putchar(conv(num&0x0F));
  putchar(' ');
}

void printascii(char num){
 
  putchar(num);
}

#endif
