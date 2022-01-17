// $Id: main.c 5048 2018-05-03 15:45:03Z ajayre $

#include "board.h"
#include "string.h"
#include "pin_mux.h"
#include "fsl_usart.h"
#include "bootloader.h"
#include "debug.h"

#define APP_BOARD_TEST_GPIO_PORT1 BOARD_LED3_GPIO_PORT
#define APP_BOARD_TEST_GPIO_PORT2 BOARD_LED1_GPIO_PORT
#define APP_BOARD_TEST_GPIO_PORT3 BOARD_LED2_GPIO_PORT
#define APP_BOARD_TEST_LED1_PIN BOARD_LED3_GPIO_PIN
#define APP_BOARD_TEST_LED2_PIN BOARD_LED1_GPIO_PIN
#define APP_BOARD_TEST_LED3_PIN BOARD_LED2_GPIO_PIN

#define TICKRATE_HZ 1000

// macros to turn LED on and off
#define LED_ON  GPIO_WritePinOutput(GPIO, APP_BOARD_TEST_GPIO_PORT1, APP_BOARD_TEST_LED1_PIN, 0)
#define LED_OFF GPIO_WritePinOutput(GPIO, APP_BOARD_TEST_GPIO_PORT1, APP_BOARD_TEST_LED1_PIN, 1)

// keeps track of the current LED state
static int LEDOn = FALSE;
// measures the passage of time
static volatile int mCounter = 0;

int main(void)
{
  usart_config_t config;

  // Define the init structure for the output LED pin
  gpio_pin_config_t led_config = {
      kGPIO_DigitalOutput, 0,
  };

  CLOCK_EnableClock(kCLOCK_Gpio0);
  CLOCK_EnableClock(kCLOCK_Gpio2);
  CLOCK_EnableClock(kCLOCK_Gpio3);
  CLOCK_AttachClk(USART_CLK_ATTACH);

  BOARD_InitPins();
  BOARD_BootClockRUN();

  // init IO for LED
  GPIO_PinInit(GPIO, APP_BOARD_TEST_GPIO_PORT1, APP_BOARD_TEST_LED1_PIN, &led_config);
  LED_OFF;

  // configure USART
  USART_GetDefaultConfig(&config);
  config.baudRate_Bps = USART_BAUDRATE;
  config.enableTx = true;
  config.enableRx = true;
  USART_Init(USART, &config, CLOCK_GetFreq(USART_CLK_SRC));
  
  // initialize bootloader
  if (!Bootloader_Init(SystemCoreClock / 1000000))
  {
    while(1);
  }
  
  // execute bootloader
	while(1)
  {
    // measure milliseconds
    // the magic number here was measured and approximates one millisecond
    // not ideal but avoids the use of interrupts in this bootloader
    if (++mCounter >= 0x189)
    {
      gBootloaderTick++;
      mCounter = 0;
    }

    // toggle LED to show we are alive
    if (gBootloaderTick % 100 == 0)
    {
      if (LEDOn)
        LED_OFF;
      else
      	LED_ON;

      LEDOn = !LEDOn;
    }
    
    // operate on the bootloader
    Bootloader_Process();
  }
}
