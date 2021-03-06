/**********************************************************************
* $Id: lpc18xx_gpio.c 2659 2012-05-17 15:13:40Z andy $		lpc18xx_gpio.c		2011-06-02
*//**
* @file		lpc18xx_gpio.c
* @brief	Contains all functions support for GPIO firmware library
* 			on LPC18xx
* @version	1.0
* @date		02. June. 2011
* @author	NXP MCU SW Application Team
*
* Copyright(C) 2011, NXP Semiconductor
* All rights reserved.
*
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup GPIO
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc18xx_gpio.h"
#include "lpc_types.h"

/* If this source file built with example, the LPC18xx FW library configuration
 * file in each example directory ("lpc18xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */
#ifdef __BUILD_WITH_EXAMPLE__
#include "lpc18xx_libcfg.h"
#else
#include "lpc18xx_libcfg_default.h"
#endif /* __BUILD_WITH_EXAMPLE__ */


#ifdef _GPIO

/* Private Functions ---------------------------------------------------------- */

//static LPC_GPIOn_Type *GPIO_GetPointer(uint8_t portNum);
//static GPIO_HalfWord_TypeDef *FIO_HalfWordGetPointer(uint8_t portNum);
//static GPIO_Byte_TypeDef *FIO_ByteGetPointer(uint8_t portNum);

#if 0
/*********************************************************************//**
 * @brief		Get pointer to GPIO peripheral due to GPIO port
 * @param[in]	portNum		Port Number value, should be in range from 0 to 4.
 * @return		Pointer to GPIO peripheral
 **********************************************************************/
static LPC_GPIOn_Type *GPIO_GetPointer(uint8_t portNum)
{
	LPC_GPIOn_Type *pGPIO = NULL;

	switch (portNum)
	{
		case 0:
			pGPIO = LPC_GPIO0;
			break;

		case 1:
			pGPIO = LPC_GPIO1;
			break;

		case 2:
			pGPIO = LPC_GPIO2;
			break;

		case 3:
			pGPIO = LPC_GPIO3;
			break;

		case 4:
			pGPIO = LPC_GPIO4;
			break;

		default:
			break;
	}

	return pGPIO;
}

/*********************************************************************//**
 * @brief		Get pointer to FIO peripheral in halfword accessible style
 * 				due to FIO port
 * @param[in]	portNum		Port Number value, should be in range from 0 to 4.
 * @return		Pointer to FIO peripheral
 **********************************************************************/
static GPIO_HalfWord_TypeDef *FIO_HalfWordGetPointer(uint8_t portNum)
{
	GPIO_HalfWord_TypeDef *pFIO = NULL;

	switch (portNum)
	{
		case 0:
			pFIO = GPIO0_HalfWord;
			break;

		case 1:
			pFIO = GPIO1_HalfWord;
			break;

		case 2:
			pFIO = GPIO2_HalfWord;
			break;

		case 3:
			pFIO = GPIO3_HalfWord;
			break;

		case 4:
			pFIO = GPIO4_HalfWord;
			break;
		default:
			break;
	}

	return pFIO;
}

/*********************************************************************//**
 * @brief		Get pointer to FIO peripheral in byte accessible style
 * 				due to FIO port
 * @param[in]	portNum		Port Number value, should be in range from 0 to 4.
 * @return		Pointer to FIO peripheral
 **********************************************************************/
static GPIO_Byte_TypeDef *FIO_ByteGetPointer(uint8_t portNum)
{
	GPIO_Byte_TypeDef *pFIO = NULL;

	switch (portNum)
	{
		case 0:
			pFIO = GPIO0_Byte;
			break;

		case 1:
			pFIO = GPIO1_Byte;
			break;

		case 2:
			pFIO = GPIO2_Byte;
			break;

		case 3:
			pFIO = GPIO3_Byte;
			break;

		case 4:
			pFIO = GPIO4_Byte;
			break;

		default:
			break;
	}

	return pFIO;
}
#endif

/* End of Private Functions --------------------------------------------------- */


/* Public Functions ----------------------------------------------------------- */
/** @addtogroup GPIO_Public_Functions
 * @{
 */


/* GPIO ------------------------------------------------------------------------------ */

/*********************************************************************//**
 * @brief		Set Direction for GPIO port.
 * @param[in]	portNum	Port Number value, should be in range from 0 to 4
 * @param[in]	bitValue	Value that contains all bits to set direction,
 * 				in range from 0 to 0xFFFFFFFF.
 * 				example: value 0x5 to set direction for bit 0 and bit 1.
 * @param[in]	dir	Direction value, should be:
 * 					- 0: Input.
 * 					- 1: Output.
 * @return		None
 *
 * Note:
 * All remaining bits that are not activated in bitValue (value '0')
 * will not be effected by this function.
 **********************************************************************/
void GPIO_SetDir(uint8_t portNum, uint32_t bitValue, uint8_t dir)
{
		if (dir)
		{
		LPC_GPIO_PORT->DIR[portNum] |= bitValue;
	} else
		{
		LPC_GPIO_PORT->DIR[portNum] &= ~bitValue;
	}
}


/*********************************************************************//**
 * @brief		Set Value for bits that have output direction on GPIO port.
 * @param[in]	portNum	Port number value, should be in range from 0 to 4
 * @param[in]	bitValue Value that contains all bits on GPIO to set, should
 * 				be in range from 0 to 0xFFFFFFFF.
 * 				example: value 0x5 to set bit 0 and bit 1.
 * @return		None
 *
 * Note:
 * - For all bits that has been set as input direction, this function will
 * not effect.
 * - For all remaining bits that are not activated in bitValue (value '0')
 * will not be effected by this function.
 **********************************************************************/
void GPIO_SetValue(uint8_t portNum, uint32_t bitValue)
{
	LPC_GPIO_PORT->SET[portNum] = bitValue;
}


/*********************************************************************//**
 * @brief		Clear Value for bits that have output direction on GPIO port.
 * @param[in]	portNum	Port number value, should be in range from 0 to 4
 * @param[in]	bitValue Value that contains all bits on GPIO to clear, should
 * 				be in range from 0 to 0xFFFFFFFF.
 * 				example: value 0x5 to clear bit 0 and bit 1.
 * @return		None
 *
 * Note:
 * - For all bits that has been set as input direction, this function will
 * not effect.
 * - For all remaining bits that are not activated in bitValue (value '0')
 * will not be effected by this function.
 **********************************************************************/
void GPIO_ClearValue(uint8_t portNum, uint32_t bitValue)
{
	LPC_GPIO_PORT->CLR[portNum] = bitValue;
}


/*********************************************************************//**
 * @brief		Read Current state on port pin that have input direction of GPIO
 * @param[in]	portNum	Port number to read value, in range from 0 to 4
 * @return		Current value of GPIO port.
 *
 * Note: Return value contain state of each port pin (bit) on that GPIO regardless
 * its direction is input or output.
 **********************************************************************/
uint32_t GPIO_ReadValue(uint8_t portNum)
{
	return LPC_GPIO_PORT->PIN[portNum];
}


#ifdef GPIO_PIN_INT
/*********************************************************************//**
 * @brief		Enable GPIO interrupt
 * @param[in]	pinInt		Pin interrupt number, should be: 0 to 7
 * @param[in]	portNum		Port number to read value, should be: 0 to 7
 * @param[in]	bitValue	Value that contains all bits on GPIO to enable,
 * 				should be in range from 0 to 0xFFFFFFFF.
 * @param[in]	intMode		interrupt mode, should be:
 * 					- 0: Rising edge interrupt mode
 * 					- 1: Falling edge interrupt mode
 * 					- 2: Active-High interrupt mode
 * 					- 3: Active-Low interrupt mode
 * @return		None
 **********************************************************************/
void GPIO_IntCmd(uint8_t pinInt, uint8_t portNum, uint32_t bitValue, uint8_t intMode)
{
	uint8_t  value;
	uint32_t pinSel;

	value = ((portNum&0x7)<<5)|(bitValue&0x1F);
	if (pinInt < 4) { //Using PINTSEL0
			pinSel = LPC_SCU->PINTSEL0;
			pinSel &= ~(0xFF<<(pinInt*8));
			pinSel |= (value<<(pinInt*8));
			LPC_SCU->PINTSEL0 = pinSel;
	} else { //Using PINTSEL1
		pinSel = LPC_SCU->PINTSEL1;
		pinSel &= ~(0xFF<<((pinInt-4)*8));
		pinSel |= (value<<((pinInt-4)*8));
		LPC_SCU->PINTSEL1 = pinSel;
	}
	switch(intMode)
	{
		case 0://rising edge
			LPC_GPIO_PIN_INT->ISEL &= ~(1<<pinInt);
			LPC_GPIO_PIN_INT->IENR |= (1<<pinInt);
			break;
		case 1://falling edge
			LPC_GPIO_PIN_INT->ISEL &= ~(1<<pinInt);
			LPC_GPIO_PIN_INT->IENF |= (1<<pinInt);
			break;
		case 2://active High level
			LPC_GPIO_PIN_INT->ISEL |= (1<<pinInt);
			LPC_GPIO_PIN_INT->IENR |= (1<<pinInt);
			LPC_GPIO_PIN_INT->SIENF |= (1<<pinInt);
			break;
		case 3://active Low level
			LPC_GPIO_PIN_INT->ISEL |= (1<<pinInt);
			LPC_GPIO_PIN_INT->IENR |= (1<<pinInt);
			LPC_GPIO_PIN_INT->CIENF |= (1<<pinInt);
			break;
		default:
			break;
	}
}


/*********************************************************************//**
 * @brief		Get GPIO Interrupt Status
 * @param[in]	pintNum	Pin interrupt number, should be: 0 to 7
 * @return		Function status,	could be:
 * 					- ENABLE	:Interrupt has been generated
 * 					- DISABLE	:Interrupt has not been detected
 **********************************************************************/
FunctionalState GPIO_GetIntStatus(uint32_t pintNum)
{
	return (((LPC_GPIO_PIN_INT->IST)>>pintNum)& 0x1);
}


/*********************************************************************//**
 * @brief		Clear GPIO interrupt status
 * @param[in]	intMode Interrupt mode, should be:
 * 					- 0: Rising edge interrupt mode
 * 					- 1: Falling edge interrupt mode
 * 					- 2: Active-High interrupt mode
 * 					- 3: Active-Low interrupt mode
 * @param[in]	pintNum Pin interrupt number, should be: 0 to 7
 * @return		None
 **********************************************************************/
void GPIO_ClearInt(uint8_t intMode, uint32_t pintNum)
{
	if (!(intMode&(1<<1)))
			LPC_GPIO_PIN_INT->IST = (1<<pintNum);
}

#endif
/* FIO word accessible ----------------------------------------------------------------- */
/* Stub function for FIO (word-accessible) style */

/**
 * @brief The same with GPIO_SetDir()
 */
void FIO_SetDir(uint8_t portNum, uint32_t bitValue, uint8_t dir)
{
	GPIO_SetDir(portNum, bitValue, dir);
}

/**
 * @brief The same with GPIO_SetValue()
 */
void FIO_SetValue(uint8_t portNum, uint32_t bitValue)
{
	GPIO_SetValue(portNum, bitValue);
}

/**
 * @brief The same with GPIO_ClearValue()
 */
void FIO_ClearValue(uint8_t portNum, uint32_t bitValue)
{
	GPIO_ClearValue(portNum, bitValue);
}

/**
 * @brief The same with GPIO_ReadValue()
 */
uint32_t FIO_ReadValue(uint8_t portNum)
{
	return (GPIO_ReadValue(portNum));
}


#ifdef GPIO_PIN_INT
/**
 * @brief The same with GPIO_IntCmd()
 */
void FIO_IntCmd(uint8_t pinInt, uint8_t portNum, uint32_t bitValue, uint8_t intMode)
{
	GPIO_IntCmd(pinInt, portNum, bitValue, intMode);
}

/**
 * @brief The same with GPIO_GetIntStatus()
 */
FunctionalState FIO_GetIntStatus(uint32_t pintNum)
{
	return (GPIO_GetIntStatus(pintNum));
}

/**
 * @brief The same with GPIO_ClearInt()
 */
void FIO_ClearInt(uint8_t intMode, uint32_t pintNum)
{
	GPIO_ClearInt(intMode, pintNum);
}
#endif


/*********************************************************************//**
 * @brief		Set mask value for bits in FIO port
 * @param[in]	portNum	Port number, in range from 0 to 4
 * @param[in]	bitValue Value that contains all bits in to set, should be
 * 				in range from 0 to 0xFFFFFFFF.
 * @param[in]	maskValue	Mask value contains state value for each bit:
 * 					- 0	:not mask.
 * 					- 1	:mask.
 * @return		None
 *
 * Note:
 * - All remaining bits that are not activated in bitValue (value '0')
 * will not be effected by this function.
 * - After executing this function, in mask register, value '0' on each bit
 * enables an access to the corresponding physical pin via a read or write access,
 * while value '1' on bit (masked) that corresponding pin will not be changed
 * with write access and if read, will not be reflected in the updated pin.
 **********************************************************************/
void FIO_SetMask(uint8_t portNum, uint32_t bitValue, uint8_t maskValue)
{
		if (maskValue)
		{
		LPC_GPIO_PORT->MASK[portNum] |= bitValue;
	} else
		{
		LPC_GPIO_PORT->MASK[portNum] &= ~bitValue;
	}
}


/* FIO halfword accessible ------------------------------------------------------------- */
#if 0
/*********************************************************************//**
 * @brief		Set direction for FIO port in halfword accessible style
 * @param[in]	portNum		Port number, in range from 0 to 4
 * @param[in]	halfwordNum	HalfWord part number, should be 0 (lower) or 1(upper)
 * @param[in]	bitValue	Value that contains all bits in to set direction,
 * 							in range from 0 to 0xFFFF.
 * @param[in]	dir	Direction value, should be:
 * 					- 0	:Input.
 * 					- 1	:Output.
 * @return		None
 *
 * Note: All remaining bits that are not activated in bitValue (value '0')
 * will not be effected by this function.
 **********************************************************************/
void FIO_HalfWordSetDir(uint8_t portNum, uint8_t halfwordNum, uint16_t bitValue, uint8_t dir)
{
	GPIO_HalfWord_TypeDef *pFIO = FIO_HalfWordGetPointer(portNum);

	if(pFIO != NULL)
	{
		// Output direction
		if (dir)
		{
			// Upper
			if(halfwordNum)
			{
				pFIO->FIODIRU |= bitValue;
			}
			// lower
			else
			{
				pFIO->FIODIRL |= bitValue;
			}
		}
		// Input direction
		else
		{
			// Upper
			if(halfwordNum)
			{
				pFIO->FIODIRU &= ~bitValue;
			}
			// lower
			else
			{
				pFIO->FIODIRL &= ~bitValue;
			}
		}
	}
}


/*********************************************************************//**
 * @brief		Set mask value for bits in FIO port in halfword accessible style
 * @param[in]	portNum		Port number, in range from 0 to 4
 * @param[in]	halfwordNum	HalfWord part number, should be 0 (lower) or 1(upper)
 * @param[in]	bitValue	Value that contains all bits in to set,
 * 							in range from 0 to 0xFFFF.
 * @param[in]	maskValue	Mask value contains state value for each bit:
 * 					- 0: not mask.
 * 					- 1: mask.
 * @return		None
 *
 * Note:
 * - All remaining bits that are not activated in bitValue (value '0')
 * will not be effected by this function.
 * - After executing this function, in mask register, value '0' on each bit
 * enables an access to the corresponding physical pin via a read or write access,
 * while value '1' on bit (masked) that corresponding pin will not be changed
 * with write access and if read, will not be reflected in the updated pin.
 **********************************************************************/
void FIO_HalfWordSetMask(uint8_t portNum, uint8_t halfwordNum, uint16_t bitValue, uint8_t maskValue)
{
	GPIO_HalfWord_TypeDef *pFIO = FIO_HalfWordGetPointer(portNum);

	if(pFIO != NULL)
	{
		// Mask
		if (maskValue)
		{
			// Upper
			if(halfwordNum)
			{
				pFIO->FIOMASKU |= bitValue;
			}
			// lower
			else
			{
				pFIO->FIOMASKL |= bitValue;
			}
		}
		// Un-mask
		else
		{
			// Upper
			if(halfwordNum)
			{
				pFIO->FIOMASKU &= ~bitValue;
			}
			// lower
			else
			{
				pFIO->FIOMASKL &= ~bitValue;
			}
		}
	}
}


/*********************************************************************//**
 * @brief		Set bits for FIO port in halfword accessible style
 * @param[in]	portNum		Port number, in range from 0 to 4
 * @param[in]	halfwordNum	HalfWord part number, should be 0 (lower) or 1(upper)
 * @param[in]	bitValue	Value that contains all bits in to set, should be
 * 				in range from 0 to 0xFFFF.
 * @return		None
 *
 * Note:
 * - For all bits that has been set as input direction, this function will
 * not effect.
 * - For all remaining bits that are not activated in bitValue (value '0')
 * will not be effected by this function.
 **********************************************************************/
void FIO_HalfWordSetValue(uint8_t portNum, uint8_t halfwordNum, uint16_t bitValue)
{
	GPIO_HalfWord_TypeDef *pFIO = FIO_HalfWordGetPointer(portNum);

	if(pFIO != NULL)
	{
		// Upper
		if(halfwordNum)
		{
			pFIO->FIOSETU = bitValue;
		}
		// lower
		else
		{
			pFIO->FIOSETL = bitValue;
		}
	}
}


/*********************************************************************//**
 * @brief		Clear bits for FIO port in halfword accessible style
 * @param[in]	portNum		Port number, in range from 0 to 4
 * @param[in]	halfwordNum	HalfWord part number, should be 0 (lower) or 1(upper)
 * @param[in]	bitValue	Value that contains all bits in to clear, should be
 * 				in range from 0 to 0xFFFF.
 * @return		None
 *
 * Note:
 * - For all bits that has been set as input direction, this function will
 * not effect.
 * - For all remaining bits that are not activated in bitValue (value '0')
 * will not be effected by this function.
 **********************************************************************/
void FIO_HalfWordClearValue(uint8_t portNum, uint8_t halfwordNum, uint16_t bitValue)
{
	GPIO_HalfWord_TypeDef *pFIO = FIO_HalfWordGetPointer(portNum);

	if(pFIO != NULL)
	{
		// Upper
		if(halfwordNum)
		{
			pFIO->FIOCLRU = bitValue;
		}
		// lower
		else
		{
			pFIO->FIOCLRL = bitValue;
		}
	}
}


/*********************************************************************//**
 * @brief		Read Current state on port pin that have input direction of GPIO
 * 				in halfword accessible style.
 * @param[in]	portNum		Port number, in range from 0 to 4
 * @param[in]	halfwordNum	HalfWord part number, should be 0 (lower) or 1(upper)
 * @return		Current value of FIO port pin of specified halfword.
 * Note: Return value contain state of each port pin (bit) on that FIO regardless
 * its direction is input or output.
 **********************************************************************/
uint16_t FIO_HalfWordReadValue(uint8_t portNum, uint8_t halfwordNum)
{
	GPIO_HalfWord_TypeDef *pFIO = FIO_HalfWordGetPointer(portNum);

	if(pFIO != NULL)
	{
		// Upper
		if(halfwordNum)
		{
			return (pFIO->FIOPINU);
		}
		// lower
		else
		{
			return (pFIO->FIOPINL);
		}
	}

	return (0);
}


/* FIO Byte accessible ------------------------------------------------------------ */

/*********************************************************************//**
 * @brief		Set direction for FIO port in byte accessible style
 * @param[in]	portNum		Port number, in range from 0 to 4
 * @param[in]	byteNum		Byte part number, should be in range from 0 to 3
 * @param[in]	bitValue	Value that contains all bits in to set direction,
 * 				in range from 0 to 0xFF.
 * @param[in]	dir	Direction value, should be:
 * 					- 0: Input.
 * 					- 1: Output.
 * @return		None
 *
 * Note: All remaining bits that are not activated in bitValue (value '0')
 * will not be effected by this function.
 **********************************************************************/
void FIO_ByteSetDir(uint8_t portNum, uint8_t byteNum, uint8_t bitValue, uint8_t dir)
{
	GPIO_Byte_TypeDef *pFIO = FIO_ByteGetPointer(portNum);

	if(pFIO != NULL)
	{
		// Output direction
		if (dir)
		{
			if (byteNum <= 3)
			{
				pFIO->FIODIR[byteNum] |= bitValue;
			}
		}
		// Input direction
		else
		{
			if (byteNum <= 3)
			{
				pFIO->FIODIR[byteNum] &= ~bitValue;
			}
		}
	}
}

/*********************************************************************//**
 * @brief		Set mask value for bits in FIO port in byte accessible style
 * @param[in]	portNum		Port number, in range from 0 to 4
 * @param[in]	byteNum		Byte part number, should be in range from 0 to 3
 * @param[in]	bitValue	Value that contains all bits in to set mask, should
 * 				be in range from 0 to 0xFF.
 * @param[in]	maskValue	Mask value contains state value for each bit:
 * 					- 0: not mask.
 * 					- 1: mask.
 * @return		None
 *
 * Note:
 * - All remaining bits that are not activated in bitValue (value '0')
 * will not be effected by this function.
 * - After executing this function, in mask register, value '0' on each bit
 * enables an access to the corresponding physical pin via a read or write access,
 * while value '1' on bit (masked) that corresponding pin will not be changed
 * with write access and if read, will not be reflected in the updated pin.
 **********************************************************************/
void FIO_ByteSetMask(uint8_t portNum, uint8_t byteNum, uint8_t bitValue, uint8_t maskValue)
{
	GPIO_Byte_TypeDef *pFIO = FIO_ByteGetPointer(portNum);

	if(pFIO != NULL)
	{
		// Mask
		if (maskValue)
		{
			if (byteNum <= 3)
			{
				pFIO->FIOMASK[byteNum] |= bitValue;
			}
		}
		// Un-mask
		else {
			if (byteNum <= 3)
			{
				pFIO->FIOMASK[byteNum] &= ~bitValue;
			}
		}
	}
}


/*********************************************************************//**
 * @brief		Set bits for FIO port in byte accessible style
 * @param[in]	portNum		Port number, in range from 0 to 4
 * @param[in]	byteNum		Byte part number, should be in range from 0 to 3
 * @param[in]	bitValue	Value that contains all bits in to set, should
 * 				be in range from 0 to 0xFF.
 * @return		None
 *
 * Note:
 * - For all bits that has been set as input direction, this function will
 * not effect.
 * - For all remaining bits that are not activated in bitValue (value '0')
 * will not be effected by this function.
 **********************************************************************/
void FIO_ByteSetValue(uint8_t portNum, uint8_t byteNum, uint8_t bitValue)
{
	GPIO_Byte_TypeDef *pFIO = FIO_ByteGetPointer(portNum);

	if (pFIO != NULL) {
		if (byteNum <= 3)
		{
			pFIO->FIOSET[byteNum] = bitValue;
		}
	}
}


/*********************************************************************//**
 * @brief		Clear bits for FIO port in byte accessible style
 * @param[in]	portNum		Port number, in range from 0 to 4
 * @param[in]	byteNum		Byte part number, should be in range from 0 to 3
 * @param[in]	bitValue	Value that contains all bits in to clear, should
 * 				be in range from 0 to 0xFF.
 * @return		None
 *
 * Note:
 * - For all bits that has been set as input direction, this function will
 * not effect.
 * - For all remaining bits that are not activated in bitValue (value '0')
 * will not be effected by this function.
 **********************************************************************/
void FIO_ByteClearValue(uint8_t portNum, uint8_t byteNum, uint8_t bitValue)
{
	GPIO_Byte_TypeDef *pFIO = FIO_ByteGetPointer(portNum);

	if (pFIO != NULL)
	{
		if (byteNum <= 3)
		{
			pFIO->FIOCLR[byteNum] = bitValue;
		}
	}
}


/*********************************************************************//**
 * @brief		Read Current state on port pin that have input direction of GPIO
 * 				in byte accessible style.
 * @param[in]	portNum		Port number, in range from 0 to 4
 * @param[in]	byteNum		Byte part number, should be in range from 0 to 3
 * @return		Current value of FIO port pin of specified byte part.
 * Note: Return value contain state of each port pin (bit) on that FIO regardless
 * its direction is input or output.
 **********************************************************************/
uint8_t FIO_ByteReadValue(uint8_t portNum, uint8_t byteNum)
{
	GPIO_Byte_TypeDef *pFIO = FIO_ByteGetPointer(portNum);

	if (pFIO != NULL)
	{
		if (byteNum <= 3)
		{
			return (pFIO->FIOPIN[byteNum]);
		}
	}
	return (0);
}
#endif

/**
 * @}
 */

#endif /* _GPIO */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
