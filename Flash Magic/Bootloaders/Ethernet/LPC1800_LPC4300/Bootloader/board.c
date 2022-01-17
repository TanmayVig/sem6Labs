/*
 * @brief Keil MCB 1857/4357 board file
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include "string.h"

#include "retarget.h"

/** @ingroup BOARD_KEIL_MCB_18574357
 * @{
 */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
/* Port and bit mapping for LEDs on GPIOs */
static const uint8_t ledports[] = {6, 6, 6, 6, 6, 4, 4, 4};
static const uint8_t ledbits[] = {24, 25, 26, 27, 28, 12, 13, 14};

/** Private definitions for LCD */
#define LCD_CS(x) ((x) ? (Chip_GPIO_SetPinState(LPC_GPIO_PORT, 7, 16, true)) : (Chip_GPIO_SetPinState(LPC_GPIO_PORT, \
																										7, 16, false)))

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/* System configuration variables used by chip driver */
const uint32_t ExtRateIn = 0;
const uint32_t OscRateIn = 12000000;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Very simple (inaccurate) delay function */
//static void delay(uint32_t i) {
//	while (i--) {}
//}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Initializes board LED(s) */
static void Board_LED_Init()
{
	int i;

	/* Must make sure J21 is installed to enabled LEDs */
	/* PD.10 : LED 0 (leftmost) */
	/* PD.11 : LED 1 */
	/* PD.12 : LED 2 */
	/* PD.13 : LED 3 */
	/* PD.14 : LED 4 */
	/* P9.0  : LED 5 */
	/* P9.1  : LED 6 */
	/* P9.2  : LED 7 (rightmost) */
	for (i = 0; i < (sizeof(ledports) / sizeof(ledports[0])); i++) {
		Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, ledports[i], ledbits[i]);
	}
}

/* Sets the state of a board LED to on or off */
void Board_LED_Set(uint8_t LEDNumber, bool On)
{
	if (LEDNumber <= 7) {
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, ledports[LEDNumber], ledbits[LEDNumber], On);
	}
}

/* Returns the current state of a board LED */
bool Board_LED_Test(uint8_t LEDNumber)
{
	if (LEDNumber <= 7) {
		return (bool) Chip_GPIO_GetPinState(LPC_GPIO_PORT, ledports[LEDNumber], ledbits[LEDNumber]);
	}
	return false;
}

void Board_LED_Toggle(uint8_t LEDNumber)
{
	Board_LED_Set(LEDNumber, !Board_LED_Test(LEDNumber));
}

/* Returns the MAC address assigned to this board */
void Board_ENET_GetMacADDR(uint8_t *mcaddr)
{
	const uint8_t boardmac[] = {0x00, 0x60, 0x37, 0x12, 0x34, 0x56};

	memcpy(mcaddr, boardmac, 6);
}

/* Set up and initialize all required blocks and functions related to the
   board hardware */
void Board_Init(void)
{
	/* Sets up DEBUG UART */
	DEBUGINIT();

	/* Initializes GPIO */
	Chip_GPIO_Init(LPC_GPIO_PORT);

	/* Setup GPIOs for USB demos */
	Chip_SCU_PinMuxSet(0x9, 5, (SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC2));			/* P9_5 USB1_VBUS_EN, USB1 VBus function */
	Chip_SCU_PinMuxSet(0x2, 5, (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC2));	/* P2_5 USB1_VBUS, MUST CONFIGURE THIS SIGNAL FOR USB1 NORMAL OPERATION */
	Chip_SCU_PinMuxSet(0x6, 3, (SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC1));			/* P6_3 USB0_PWR_EN, USB0 VBus function */
	
	/* Initialize LEDs */
	Board_LED_Init();
#if defined(USE_RMII)
	Chip_ENET_RMIIEnable(LPC_ETHERNET);
#else
	Chip_ENET_MIIEnable(LPC_ETHERNET);
#endif
}

/**
 * @}
 */
