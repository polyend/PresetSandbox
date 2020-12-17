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

#include "mtLED.h"
#include <i2c_t3.h>

mtLED presetLeds;

#define ISSI_ADDR_DEFAULT 0xE8

#define ISSI_REG_CONFIG  0x00
#define ISSI_REG_CONFIG_PICTUREMODE 0x00
#define ISSI_REG_CONFIG_AUTOPLAYMODE 0x08
#define ISSI_REG_CONFIG_AUDIOPLAYMODE 0x18

#define ISSI_CONF_PICTUREMODE 0x00
#define ISSI_CONF_AUTOFRAMEMODE 0x04
#define ISSI_CONF_AUDIOMODE 0x08

#define ISSI_REG_PICTUREFRAME  0x01

#define ISSI_REG_SHUTDOWN 0x0A
#define ISSI_REG_AUDIOSYNC 0x06

#define ISSI_COMMANDREGISTER 0xFD
#define ISSI_BANK_FUNCTIONREG 0x0B    // helpfully called 'page nine'

#define I2C_MAX_WAITING 10000

mtLED::mtLED()
{
	memset(ledState, 0, 144);
	memset(ledBits, 0, 18);
	memset(ledPWM, 0, 144);
}

void mtLED::init(uint8_t addr)
{
#ifdef mtLED_I2C_DMA
	Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000, I2C_OP_MODE_DMA);
#else
	Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 350000);
#endif
	Wire.setDefaultTimeout(1000); // 250ms default timeout

	i2cAddr = addr;
	frame = 0;

	// shutdown
	writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_SHUTDOWN, 0x00);

	delay(10);

	// out of shutdown
	writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_SHUTDOWN, 0x01);

	// picture mode
	writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_CONFIG,
					ISSI_REG_CONFIG_PICTUREMODE);

	displayFrame(frame);

	// all LEDs on & 0 PWM
	setLEDOnOffBytes(ledBits, frame);

	clear(); // set each led to 0 PWM

	memset(ledPWM, 0, 144);
	memset(ledBits, 0, 18);

	ledBits[1] = 1;
	ledBits[17] = 255;

	ledPWM[8] = 255;
	for (uint8_t i = 136; i < 144; i++)
	{
		ledPWM[i] = 255;
	}
	updateFlag = 1;
}

void mtLED::setEncoderColor(uint8_t num, uint8_t state, uint16_t color,
							uint8_t power) //power 0 - 100
{
	uint8_t green_gamma_pwm = color >> 8;
	uint8_t blue_gamma_pwm = color;
	uint8_t stateGreen = state;
	uint8_t stateBlue = state;

	green_gamma_pwm = (uint8_t)((float) green_gamma_pwm * (power / 100.0));
	blue_gamma_pwm = (uint8_t)((float) blue_gamma_pwm * (power / 100.0));

	if (green_gamma_pwm == 0) stateGreen = 0;
	if (blue_gamma_pwm == 0) stateBlue = 0;

	switch (num)
	{
	case 0:
		setLED(cgreen1, stateGreen, green_gamma_pwm);
		setLED(cblue1, stateBlue, blue_gamma_pwm);
		break;
	case 1:
		setLED(cgreen2, stateGreen, green_gamma_pwm);
		setLED(cblue2, stateBlue, blue_gamma_pwm);
		break;
	case 2:
		setLED(cgreen3, stateGreen, green_gamma_pwm);
		setLED(cblue3, stateBlue, blue_gamma_pwm);
		break;
	case 3:
		setLED(cgreen4, stateGreen, green_gamma_pwm);
		setLED(cblue4, stateBlue, blue_gamma_pwm);
		break;
	case 4:
		setLED(cgreen5, stateGreen, green_gamma_pwm);
		setLED(cblue5, stateBlue, blue_gamma_pwm);
		break;
	case 5:
		setLED(cgreen6, stateGreen, green_gamma_pwm);
		setLED(cblue6, stateBlue, blue_gamma_pwm);
		break;
	case 6:
		setLED(cgreen7, stateGreen, green_gamma_pwm);
		setLED(cblue7, stateBlue, blue_gamma_pwm);
		break;
	case 7:
		setLED(cgreen8, stateGreen, green_gamma_pwm);
		setLED(cblue8, stateBlue, blue_gamma_pwm);
		break;
	case 8:
		setLED(cgreen9, stateGreen, green_gamma_pwm);
		setLED(cblue9, stateBlue, blue_gamma_pwm);
		break;

	default:
		break;
	}
}

void mtLED::update()
{
	if (!Wire.done())
	{
		return;
	}
	
	if (updateFlag)
	{
		checkAndRepair();

		Wire.beginTransmission(i2cAddr);
		Wire.write((byte) ISSI_COMMANDREGISTER);
		Wire.write(frame);
		Wire.endTransmission();

		Wire.beginTransmission(i2cAddr);
		Wire.write((byte) 0x00);

		for (uint8_t i = 0; i < 18; i++)  //0x11
		{
			Wire.write(ledBits[i]);
		}

		for (uint8_t i = 0; i < 18; i++)  //0x11
		{
			Wire.write(0);
		}

		for (uint8_t i = 0; i < 144; i++)
		{
			Wire.write((byte) ledPWM[i]);
		}

		Wire.sendTransmission();

		updateFlag = 0;
	}
}

void mtLED::setLED(uint8_t num, uint8_t state, uint8_t gamma_pwm)
{
	if (gamma_pwm > 31) return;
	ledPWM[num] = gamma32[gamma_pwm];
	if (state) ledBits[num / 8] |= (1 << num % 8);
	else
		ledBits[num / 8] &= ~(1 << num % 8);
	updateFlag = 1;
}

void mtLED::clear(void)
{
	// all LEDs on & 0 PWM
	selectBank(frame);

	for (uint8_t i = 0; i < 6; i++)
	{
		Wire.beginTransmission(i2cAddr);
		Wire.write((byte) 0x24 + i * 24);
		for (uint8_t j = 0; j < 18; j++)
		{
			Wire.write((byte) 0);
		}
		Wire.endTransmission();
	}
}

void mtLED::setLEDOnOffBytes(uint8_t *arr, uint8_t bank)
{
	Wire.beginTransmission(i2cAddr);
	Wire.write((uint8_t) ISSI_COMMANDREGISTER);
	Wire.write((uint8_t) bank);
	Wire.endTransmission();

	Wire.beginTransmission(i2cAddr);
	Wire.write((byte) 0x00);
	for (uint8_t i = 0; i < 18; i++)  //0x11
	{
		Wire.write(arr[i]);
	}
	Wire.endTransmission();
}

void mtLED::displayFrame(uint8_t f)
{
	if (f > 7) f = 0;
	writeRegister8(ISSI_BANK_FUNCTIONREG, ISSI_REG_PICTUREFRAME, f);
}

void mtLED::selectBank(uint8_t b)
{
	Wire.beginTransmission(i2cAddr);
	Wire.write((uint8_t) ISSI_COMMANDREGISTER);
	Wire.write((uint8_t) b);
	Wire.endTransmission();
}

void mtLED::writeRegister8(uint8_t b, uint8_t reg, uint8_t data)
{

	Wire.beginTransmission(i2cAddr);
	Wire.write((byte) ISSI_COMMANDREGISTER);
	Wire.write((byte) b);
	Wire.endTransmission();

	Wire.beginTransmission(i2cAddr);
	Wire.write((byte) reg);
	Wire.write((byte) data);
	Wire.endTransmission();

	if (b == ISSI_BANK_FUNCTIONREG)
	{
		functionRegBuff[reg] = data;
	}
}

uint8_t mtLED::readRegister8(uint8_t bank, uint8_t reg)
{
	uint16_t _count = 0;
	uint8_t x;
	x = 0;

	while (!Wire.done())
		_count++; // Since write is non-blocking, do some counting while waiting

	Wire.beginTransmission(i2cAddr);
	Wire.write((byte) ISSI_COMMANDREGISTER);
	Wire.write((byte) bank);
	Wire.endTransmission();

	Wire.beginTransmission(i2cAddr);
	Wire.write((byte) reg);
	Wire.endTransmission();

	Wire.requestFrom(i2cAddr, uint8_t(1));

	while (Wire.available())
	{
		x = Wire.read();
	}
	Wire.endTransmission();

	return x;
}

bool mtLED::checkAndRepairReg(uint8_t reg)
{
	if (readRegister8(ISSI_BANK_FUNCTIONREG, reg) != functionRegBuff[reg])
	{
		writeRegister8(ISSI_BANK_FUNCTIONREG, reg, functionRegBuff[reg]);
		return 1;
	}

	return 0;
}

void mtLED::checkAndRepair(void)
{
	for (uint8_t reg = 0; reg <= 0x0D; reg++)
	{
		// omijamy wyjątki:
		if (reg == 0x07) continue;

		checkAndRepairReg(reg);
	}
}

void mtLED::setPresetGridLED(uint8_t n, uint8_t state, uint8_t gamma_pwm)
{
	setLED(((n / 8) * 8) * 2 + n % 8, state, gamma_pwm);
}

void mtLED::checkFault()
{
	static uint32_t sendCounter;
	if (Wire.status() == 8) sendCounter++;
	else
		sendCounter = 0;

	if (sendCounter > 100)
	{
		SIM_SCGC4 &= ~SIM_SCGC4_I2C0;
		delayMicroseconds(10);
		SIM_SCGC4 |= SIM_SCGC4_I2C0;
		Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 350000);
		sendCounter = 0;
	}
}
