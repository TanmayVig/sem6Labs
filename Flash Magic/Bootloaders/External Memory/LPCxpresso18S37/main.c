// main - initializes and executes bootloader
// and keeps track of time
// $Id: main.c 3893 2015-05-12 11:46:44Z andy $

#include "lpc18xx_cgu.h"
#include "bootloader.h"

// system tick interrupt handler
// must be called every millisecond so the bootloader can keep track
// of time
void SysTick_Handler(void)
{
	gBootloaderTick++;
}

// main program body
int c_entry(void)
{
  // we need a system tick every millisecond  
  SysTick_Config(SystemCoreClock/10);
  
  // initialize bootloader
  if (!Bootloader_Init(96))
  {
    while(1);
  }

  // execute bootloader
	while(1)
  {    
    Bootloader_Process();
  }
}

// With ARM and GHS toolsets, the entry point is main() - this will
// allow the linker to generate wrapper code to setup stacks, allocate
// heap area, and initialize and copy code and data segments. For GNU
// toolsets, the entry point is through __start() in the crt0_gnu.asm
// file, and that startup code will setup stacks and data
int main(void)
{
  return c_entry();
}
