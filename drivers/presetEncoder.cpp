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

#include "presetEncoder.h"
#include "Arduino.h"
#include "presetMultiplex.h"
#include "presetHardware.h"

IntervalTimer encoderInterrupt;

static constexpr int encodersCount = 9;

struct
{
	volatile uint8_t a;
	volatile uint8_t b;

} encoders[encodersCount];

static const uint8_t encMultiA[] = { 0, 2, 4, 6, 8, 10, 12, 14 };
static const uint8_t encMultiB[] = { 1, 3, 5, 7, 9, 11, 13, 15 };

static void (*handlerFunc)(uint8_t, uint8_t)=NULL;

void updateEncoders();

void initEncoders(void (*handler)(uint8_t, uint8_t))
{
	handlerFunc = handler;

	encoderInterrupt.begin(updateEncoders, 1000);
}

void updateEncoders()
{
	for (int i = 0; i < encodersCount - 1; i++)
	{
		setMultiplexEncoders(encMultiA[i]);
		encoders[i].a = (encoders[i].a << 1) | digitalRead(MUX_OUT);
		setMultiplexEncoders(encMultiB[i]);
		encoders[i].b = (encoders[i].b << 1) | digitalRead(MUX_OUT);
	}
	setMultiplexSwitch(14);
	encoders[8].a = (encoders[8].a << 1) | digitalRead(MUX_OUT_2);
	setMultiplexSwitch(15);
	encoders[8].b = (encoders[8].b << 1) | digitalRead(MUX_OUT_2);

	for (int i = 0; i < encodersCount; i++)
	{
		if ((encoders[i].a & 0x03) == 0x02 && (encoders[i].b & 0x03) == 0x00)
		{
			(*handlerFunc)(i, RIGHT);
		}
		else if ((encoders[i].b & 0x03) == 0x02 && (encoders[i].a & 0x03) == 0x00)
		{
			(*handlerFunc)(i, LEFT);
		}

	}
}

