/* Polyend Preset Sandbox
 *
 * Copyright (c) 2020 Polyend
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef DRIVERS_PRESETHARDWARE_H_
#define DRIVERS_PRESETHARDWARE_H_

//MUX SWITCH
#define MUX_CTRL_2A		15		//PTC0
#define MUX_CTRL_2B		22		//PTC1
#define MUX_CTRL_2C		23		//PTC2
#define MUX_CTRL_2D		9		//PTC3

#define MUX_OUT_2		25		//PTD19
//MUX ENCODERS

#define MUX_CTRL_A		6		//PTD4
#define MUX_CTRL_B		20		//PTD5
#define MUX_CTRL_C		21		//PTD6
#define MUX_CTRL_D		5		//PTD7


#define MUX_OUT			2		//PTD0

#define BLINK_LED 		17		//PTB1

#define SPI_SCK			13		//PTC5
#define SPI_MOSI		11		//PTC6
#define SPI_MISO		12		//PTC7
#define DAC_SPI_CS		7		//PTD2
#define FLASH_SPI_CS	8		//PTD3
#define DAC_INT			14		//PTD1

#define LED_I2C_SCL		19		//PTB2
#define LED_I2C_SDA		18		//PTB3

#define BUTT_A 			A12
#define BUTT_B			A13
#define BUTT_C			A10
#define BUTT_D			A11

#define BUTT_HC595_SER	3		//PTA12
#define BUTT_HC595_SCK	4		//PTA13
#define BUTT_HC595_RCK	16		//PTB0

#define BANK_SWITCH		2
#define	REC_SWITCH		1
#define CLEAR_SWITCH	0

#define CALIB_PIN		32

#define IS31FL3731_ADDR1 0b1110100

#endif /* DRIVERS_PRESETHARDWARE_H_ */
