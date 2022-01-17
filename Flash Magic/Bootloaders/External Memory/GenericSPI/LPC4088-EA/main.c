// main - initializes and executes bootloader
// and keeps track of time
// $Id: main.c 3004 2013-01-24 14:08:14Z andy $

#include "lpc_clkpwr.h"
#include "lpc_systick.h"
#include "lpc_nvic.h"
#include "bootloader.h"

// system tick interrupt handler
// must be called every millisecond so the bootloader can keep track
// of time
void SysTick_Handler(void)
{
  gBootloaderTick++;
}

/*********************************************************************//**
 * @brief       Main program body
 * @param[in]   None
 * @return      int
 **********************************************************************/
int c_entry(void)
{
  // set vector table to RAM
  // this address must match the start address for the linker
  NVIC_SetVTOR(0x10000500);
  
  SYSTICK_InternalInit(1);
  SYSTICK_Cmd(ENABLE);
  SYSTICK_IntCmd(ENABLE);

  // initialize bootloader
  if (!Bootloader_Init(120))
  {
    while(1);
  }

  while (1)
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
