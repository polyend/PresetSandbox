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

#ifndef _MTLED_H_
#define _MTLED_H_

#include <stdint.h>

#define GREEN	(31<<8)
#define BLUE	(31)
#define CYAN1	((31<<8)|31)
#define CYAN2	((31<<8)|15)

class mtLED
{
public:
	static constexpr uint8_t maxGammaPwm = 31;

	mtLED();
	void init(uint8_t addr);
	void update();
	void setLED(uint8_t num, uint8_t state, uint8_t gamma_pwm);
	void setEncoderColor(uint8_t num, uint8_t state, uint16_t color,
							uint8_t power);
	void setPresetGridLED(uint8_t n, uint8_t state, uint8_t gamma_pwm);
	void checkFault();

private:
	void clear(void);
	void selectBank(uint8_t bank);
	uint8_t readRegister8(uint8_t bank, uint8_t reg);
	void writeRegister8(uint8_t bank, uint8_t reg, uint8_t data);
	bool checkAndRepairReg(uint8_t reg);
	void checkAndRepair(void);
	void displayFrame(uint8_t frame);
	void setLEDOnOffBytes(uint8_t *arr, uint8_t bank);

	uint8_t i2cAddr;
	uint8_t frame;

	uint8_t gamma32[32] = { 0, 1, 2, 4, 6, 10, 13, 18, 22, 28, 33, 39, 46, 53, 61, 69, 78, 86, 96, 106, 116, 126, 138, 149, 161, 173, 186, 199, 212, 226, 240, 255 };

	uint8_t ledState[144];
	uint8_t updateFlag = 0;

	uint8_t ledBits[18] =
			{	//---A----  ----B-----
			0b00000000, 0b00000000,
					0b00000000, 0b00000000,
					0b00000000, 0b00000000,
					0b00000000, 0b00000000,
					0b00000000, 0b00000000,
					0b00000000, 0b00000000,
					0b00000000, 0b00000000,
					0b00000000, 0b00000000,
					0b00000000, 0b00000000
			};

	uint8_t ledPWM[144] =
			{
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 0, 0, 0
			};

	// default values
	uint8_t functionRegBuff[0x0D] =
			{
					0x00, //  0x00
					0x00, //  0x01
					0x00, //  0x02
					0x00, //  0x03
					0x00, //  0x04
					0x00, //  0x05
					0x00, //  0x06
					0x00, //  0x07
					0x00, //  0x08
					0x00, //  0x09
					0x00, //  0x0A
					0x00, //  0x0B
					0x00 //  0x0C
			};

	typedef enum colors
	{
		cgreen1 = 120,
		cgreen2,
		cgreen3,
		cgreen4,
		cgreen5,
		cgreen6,
		cgreen7,
		cgreen8 = 110,
		cgreen9 = 41,
		cblue1 = 104,
		cblue2,
		cblue3,
		cblue4,
		cblue5,
		cblue6,
		cblue7 = 93,
		cblue8 = 94,
		cblue9 = 57,
	} enc_colors_t;
};

extern mtLED presetLeds;

#endif
