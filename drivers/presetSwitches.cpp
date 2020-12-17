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

#include "presetSwitches.h"
#include "presetHardware.h"
#include "presetMultiplex.h"

presetEncoderSwitches encoderSwitches;
presetTactSwitches tactSwitches;

void presetEncoderSwitches::init()
{
	for (uint8_t i = 0; i < 9; i++)
	{
		offTimer[i] = 0;
		setMultiplexSwitch(i);
		delayMicroseconds(5);
		lastState[i] = digitalRead(MUX_OUT_2);
		madeReleaseFlag[i] = 1;
	}
}

void presetEncoderSwitches::update()
{
	for (uint8_t i = 0; i < 9; i++)
	{
		noInterrupts();
		setMultiplexSwitch(i);
		delayMicroseconds(5);
		currentState[i] = digitalRead(MUX_OUT_2);
		interrupts();

		if ((!currentState[i]) || (currentState[i] != lastState[i]))
		{
			offMeasureFlag[i] = 0;
			if (!madePressFlag[i])
			{
				madePressFlag[i] = 1;
				madeReleaseFlag[i] = 0;
				pressAction(i);
			}
		}

		if (!offMeasureFlag[i]) offTimer[i] = 0;
		if ((currentState[i] == lastState[i]) && currentState[i])
		{
			offMeasureFlag[i] = 1;
		}

		if (offTimer[i] > 100)
		{
			if (offTimer[i] > 4000000000) offTimer[i] = 101;
			offMeasureFlag[i] = 0;
			if (!madeReleaseFlag[i])
			{
				madeReleaseFlag[i] = 1;
				madePressFlag[i] = 0;
				releaseAction(i);
			}
		}

		lastState[i] = currentState[i];
	}

}

void presetEncoderSwitches::setPressFunction(void (*func)(uint8_t n))
{
	pressAction = func;
}

void presetEncoderSwitches::setReleaseFunction(void (*func)(uint8_t n))
{
	releaseAction = func;
}

void presetTactSwitches::init()
{
	for (uint8_t i = 0; i < 3; i++)
	{
		offTimer[i] = 0;
		madeReleaseFlag[i] = 1;
	}
}

void presetTactSwitches::update()
{
	for (uint8_t i = 0; i < 3; i++)
	{
		noInterrupts();
		setMultiplexSwitch(i + 9);
		currentState[i] = digitalRead(MUX_OUT_2);
		interrupts();

		if (currentState[i])
		{
			offMeasureFlag[i] = 0;
			if (!madePressFlag[i])
			{
				madePressFlag[i] = 1;
				madeReleaseFlag[i] = 0;
				pressAction(i);
			}
		}

		if (!offMeasureFlag[i]) offTimer[i] = 0;
		if (!currentState[i])
		{
			offMeasureFlag[i] = 1;
		}

		if (offTimer[i] > 10)
		{
			if (offTimer[i] > 4000000000) offTimer[i] = 11;
			offMeasureFlag[i] = 0;
			if (!madeReleaseFlag[i])
			{
				madeReleaseFlag[i] = 1;
				madePressFlag[i] = 0;
				releaseAction(i);
			}
		}
	}

}

void presetTactSwitches::setPressFunction(void (*func)(uint8_t n))
{
	pressAction = func;
}

void presetTactSwitches::setReleaseFunction(void (*func)(uint8_t n))
{
	releaseAction = func;
}
