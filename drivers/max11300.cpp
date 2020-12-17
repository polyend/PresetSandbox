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

#include <max11300.h>
#include <max11300registers.h>
#include "Arduino.h"
#include "SPI.h"

/* DAC MODES DEFINES*/
#define DAC_5_5 20992
#define DAC_0_10 20736
#define DAC_0_10wADC128 27104
#define ADC_0_10 29024
#define DAC_10_0 21248

/*Voltage defines*/
#define VOL_0V 0
#define VOL_5V 5
#define VOL_10V 10

#define DAC_MAX_VALUE 4095U
#define DAC_MIN_VALUE 0U

#define SPIsett SPISettings(4000000, MSBFIRST, SPI_MODE0)

const int INTB = 14;
const int slaveSelectPin = 7;

static int analogStatus;
static uint16_t deviceId;

int max_readRegister(uint8_t address)
{
	int data;
	digitalWrite(slaveSelectPin, LOW);

	SPI.transfer((address << 1) | 1);
	data = (SPI.transfer(0) << 8);
	data |= (SPI.transfer(0) & 0xff);

	digitalWrite(slaveSelectPin, HIGH);

	return data;
}

void max_writeRegister(uint8_t address, uint16_t data)
{
	digitalWrite(slaveSelectPin, LOW);

	SPI.begin();
	SPI.beginTransaction(SPIsett);

	SPI.transfer(address << 1);
	SPI.transfer((data >> 8) & 0xff);
	SPI.transfer(data & 0xff);

	digitalWrite(slaveSelectPin, HIGH);
	SPI.endTransaction();
}


void max_burstWriteRegister(uint8_t address, uint16_t * data, uint8_t size)
{
	SPI.begin();
	SPI.beginTransaction(SPIsett);
	digitalWrite(slaveSelectPin, LOW);

	SPI.transfer(address << 1);
	for (uint8_t i = 0; i < size; i++)
	{
		SPI.transfer((data[i] >> 8) & 0xff);
		SPI.transfer(data[i] & 0xff);
	}
	digitalWrite(slaveSelectPin, HIGH);
	SPI.endTransaction();
}

void max_burstReadRegister(uint8_t address,uint16_t *data, uint8_t size)
{
	SPI.begin();
	SPI.beginTransaction(SPIsett);

	digitalWrite(slaveSelectPin, LOW);

	SPI.transfer((address << 1) | 1);
	for(uint8_t i=0;i<20;i++)
	{
		*data=(SPI.transfer(0) << 8);
		*data |= (SPI.transfer(0) & 0xff);

		data++;
	}
	digitalWrite(slaveSelectPin, HIGH);

	SPI.endTransaction();

}

void max_init()
{
	uint16_t temp;

	pinMode(slaveSelectPin, OUTPUT);
	pinMode(INTB, INPUT_PULLUP);
	digitalWrite(slaveSelectPin, HIGH);

	SPI.begin();

	while(max_readRegister(0x00) != 0x424)
	{
		delay(10);

		pinMode(slaveSelectPin, OUTPUT);
		pinMode(INTB, INPUT_PULLUP);
		digitalWrite(slaveSelectPin, HIGH);
		SPI.begin();
		delay(100);

	}
	deviceId = max_readRegister(0x00);
	max_writeRegister(0x10, 0x8000);//reset

	delay(100);

	for (int i = 0; i < 20; i++)//przejscie na wysoka impedancje
	{
		max_writeRegister(0x20 + i, 0);
		// delay(1);
	}

	max_writeRegister(0x10, 0b01000000);  // DACREF internal ref
	// delay(1);

	for(int i=0;i<20;i++)
	{
		max_writeRegister(0x60+i,0);
	}

	for (int i = 0; i < 9; i++)
	{
		max_writeRegister(0x21 + i, DAC_0_10);
		// delay(1);
	}

	max_writeRegister(0x20, ADC_0_10);

	temp=max_readRegister(0x10);
	temp |= 0x03;
	max_writeRegister(0x10,temp);
}

uint16_t max_readADC(uint8_t pin)
{
	analogStatus &= ~(1 << pin);
	return max_readRegister(0x40+pin);
}

bool max_isADCReady(uint8_t pin)
{
	if(max_readRegister(MAX_INT) & MAX_ADCDR_MASK)
	{
		analogStatus |= (((uint32_t)(max_readRegister(MAX_ADCST_H))) << 16) | (max_readRegister(MAX_ADCST_L));
	}

	if(analogStatus & (1 << pin))
	{
		return true;
	}

	return false;
}

void max_writeDAC(uint16_t val,uint8_t cvnum)
{
	digitalWrite(slaveSelectPin, LOW);

	SPI.begin();
	SPI.beginTransaction(SPIsett);

	SPI.transfer((0x60+(cvnum+1)) << 1);
	SPI.transfer((val>> 8) & 0xff);
	SPI.transfer(val  & 0xff);

	digitalWrite(slaveSelectPin, HIGH);

	SPI.endTransaction();
}




