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

#include "hc595.h"

ShiftRegisterHC595 shiftRegisterHC595;

void ShiftRegisterHC595::init(uint8_t clock_pin, uint8_t data_pin,
								uint8_t cs_pin)
{
	_clock_pin = clock_pin;
	_data_pin = data_pin;
	_cs_pin = cs_pin;

	pinMode(_clock_pin, OUTPUT);
	pinMode(_data_pin, OUTPUT);
	pinMode(_cs_pin, OUTPUT);

	digitalWrite(_clock_pin, 1);
	digitalWrite(_cs_pin, 0);

	clearRegister();
}

void ShiftRegisterHC595::shift(uint8_t data_bit)
{
	digitalWrite(_cs_pin, 0);
	digitalWrite(_clock_pin, 0);

	digitalWrite(_data_pin, data_bit);

	digitalWrite(_clock_pin, 1);
	digitalWrite(_cs_pin, 1);
}

void ShiftRegisterHC595::clearRegister()
{
	digitalWrite(_cs_pin, 0);

	for (uint8_t i = 0; i < 8; i++)
	{
		digitalWrite(_clock_pin, 0);
		digitalWrite(_data_pin, 0);
		digitalWrite(_clock_pin, 1);
	}
	digitalWrite(_cs_pin, 1);
}

void ShiftRegisterHC595::update()
{
	shift(1);
	for (uint8_t i = 0; i < 8; i++)
	{
		measureAction(i);
		shift(0);
	}
}

void ShiftRegisterHC595::setMeasureFunc(void (*func)(uint8_t))
{
	measureAction = func;
}
