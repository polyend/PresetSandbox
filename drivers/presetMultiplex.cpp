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

#include "presetMultiplex.h"
#include "presetHardware.h"
#include "Arduino.h"

void initMultiplex()
{
	pinMode(MUX_CTRL_2A, OUTPUT);
	pinMode(MUX_CTRL_2B, OUTPUT);
	pinMode(MUX_CTRL_2C, OUTPUT);
	pinMode(MUX_CTRL_2D, OUTPUT);
	pinMode(MUX_OUT_2, INPUT_PULLUP);

	pinMode(MUX_CTRL_A, OUTPUT);
	pinMode(MUX_CTRL_B, OUTPUT);
	pinMode(MUX_CTRL_C, OUTPUT);
	pinMode(MUX_CTRL_D, OUTPUT);
	pinMode(MUX_OUT, INPUT_PULLUP);
}

void setMultiplexSwitch(uint8_t value)
{

	if (value & 1) digitalWrite(MUX_CTRL_2A, 1);
	else
		digitalWrite(MUX_CTRL_2A, 0);
	if (value & 2) digitalWrite(MUX_CTRL_2B, 1);
	else
		digitalWrite(MUX_CTRL_2B, 0);
	if (value & 4) digitalWrite(MUX_CTRL_2C, 1);
	else
		digitalWrite(MUX_CTRL_2C, 0);
	if (value & 8) digitalWrite(MUX_CTRL_2D, 1);
	else
		digitalWrite(MUX_CTRL_2D, 0);
}

void setMultiplexEncoders(uint8_t value)
{
	if (value & 1) digitalWrite(MUX_CTRL_A, 1);
	else
		digitalWrite(MUX_CTRL_A, 0);
	if (value & 2) digitalWrite(MUX_CTRL_B, 1);
	else
		digitalWrite(MUX_CTRL_B, 0);
	if (value & 4) digitalWrite(MUX_CTRL_C, 1);
	else
		digitalWrite(MUX_CTRL_C, 0);
	if (value & 8) digitalWrite(MUX_CTRL_D, 1);
	else
		digitalWrite(MUX_CTRL_D, 0);
}

