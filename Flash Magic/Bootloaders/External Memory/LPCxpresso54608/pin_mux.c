/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "pin_mux.h"
#include "fsl_common.h"
#include "fsl_iocon.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_InitPins(void)
{
    /* enable clock for IOCON */
    CLOCK_EnableClock(kCLOCK_Iocon);

    /* SPI9 pins - master */
    IOCON_PinMuxSet(IOCON, 0, 23, (IOCON_FUNC6 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN));
    IOCON_PinMuxSet(IOCON, 0, 24, (IOCON_FUNC6 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN));  /* SPI9_SSEL0 */
    IOCON_PinMuxSet(IOCON, 0, 25, (IOCON_FUNC6 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN));  /* SPI9_SCK */
    IOCON_PinMuxSet(IOCON, 0, 26, (IOCON_FUNC6 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN));  /* SPI9_MOSI */
    IOCON_PinMuxSet(IOCON, 0, 27, (IOCON_FUNC6 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN));  /* SPI9_MISO */
    IOCON_PinMuxSet(IOCON, 0, 28, (IOCON_FUNC6 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN));  /* SPI9_MISO */

    /* USART0 RX/TX pins */
    IOCON_PinMuxSet(IOCON, 0, 29, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);
    IOCON_PinMuxSet(IOCON, 0, 30, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);

    /* USART8 RX/TX pins */
    IOCON_PinMuxSet(IOCON, 2, 17, IOCON_MODE_INACT | IOCON_FUNC5 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);
  	IOCON_PinMuxSet(IOCON, 3, 17, IOCON_MODE_INACT | IOCON_FUNC1 | IOCON_DIGITAL_EN | IOCON_INPFILT_OFF);
}
