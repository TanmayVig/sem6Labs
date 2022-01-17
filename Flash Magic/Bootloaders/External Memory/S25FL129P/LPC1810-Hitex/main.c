// main - initializes and executes bootloader
// and keeps track of time
// $Id: main.c 3557 2014-04-28 11:24:48Z andy $

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
	SystemInit();
	CGU_Init();

	// M3Frequency is automatically set when SetClock(BASE_M3_CLK... was called
  // Generate interrupt @ 1000 Hz
	SysTick_Config(CGU_GetPCLKFrequency(CGU_PERIPHERAL_M3CORE)/1000);  				

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
