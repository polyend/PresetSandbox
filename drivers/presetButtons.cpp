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

#include "presetButtons.h"
#include "hc595.h"
#include "presetHardware.h"

#define THRESHOLD_ON 200

elapsedMillis offTimerGridButtons[32];
uint8_t currentStateGridButtons[32];
uint8_t offMeasureFlagGridButtons[32];
uint8_t madePressFlagGridButtons[32];
uint8_t madeReleaseFlagGridButtons[32];
uint8_t firstUpdate = 1;
void (*pressActionGridButtons)(uint8_t);
void (*releaseActionGridButtons)(uint8_t);
void measureGridButtons(uint8_t multiplexNumber);

void initGridButtons()
{
	shiftRegisterHC595.setMeasureFunc(measureGridButtons);
	shiftRegisterHC595.init(BUTT_HC595_SCK, BUTT_HC595_SER, BUTT_HC595_RCK);
	analogReadResolution(10);

	for (uint8_t i = 0; i < 32; i++)
	{
		offTimerGridButtons[i] = 0;
		madeReleaseFlagGridButtons[i] = 1;
	}
}

void updateGridButtons()
{
	shiftRegisterHC595.update();
}

void setPressActionGridButtons(void (*func)(uint8_t))
{
	pressActionGridButtons = func;
}

void setReleaseActionGridButtons(void (*func)(uint8_t))
{
	releaseActionGridButtons = func;
}

void measureGridButtons(uint8_t multiplexNumber)
{
	uint8_t calcNumber = 0;
	for (uint8_t i = 0; i < 4; i++)
	{
		calcNumber = multiplexNumber * 4 + i;
		switch (i)
		{
		case 0:
			if (analogRead(BUTT_A) > THRESHOLD_ON) currentStateGridButtons[calcNumber] = 1;
			else
				currentStateGridButtons[multiplexNumber * 4 + i] = 0;
			break;
		case 1:
			if (analogRead(BUTT_B) > THRESHOLD_ON) currentStateGridButtons[calcNumber] = 1;
			else
				currentStateGridButtons[calcNumber] = 0;
			break;
		case 2:
			if (analogRead(BUTT_C) > THRESHOLD_ON) currentStateGridButtons[calcNumber] = 1;
			else
				currentStateGridButtons[calcNumber] = 0;
			break;
		case 3:
			if (analogRead(BUTT_D) > THRESHOLD_ON) currentStateGridButtons[calcNumber] = 1;
			else
				currentStateGridButtons[calcNumber] = 0;
			break;
		}

		if (currentStateGridButtons[calcNumber])
		{
			offMeasureFlagGridButtons[calcNumber] = 0;

			if (!madePressFlagGridButtons[calcNumber])
			{
				madePressFlagGridButtons[calcNumber] = 1;
				madeReleaseFlagGridButtons[calcNumber] = 0;

				pressActionGridButtons((calcNumber % 4) * 8 + calcNumber / 4);
			}
		}

		if (!offMeasureFlagGridButtons[calcNumber])
			offTimerGridButtons[calcNumber] = 0;
		if (!currentStateGridButtons[calcNumber])
		{
			offMeasureFlagGridButtons[calcNumber] = 1;
		}

		if (offTimerGridButtons[calcNumber] > 10)
		{
			if (offTimerGridButtons[calcNumber] > 4000000000)
				offTimerGridButtons[calcNumber] = 11;

			offMeasureFlagGridButtons[calcNumber] = 0;

			if (!madeReleaseFlagGridButtons[calcNumber])
			{
				madeReleaseFlagGridButtons[calcNumber] = 1;
				madePressFlagGridButtons[calcNumber] = 0;
				releaseActionGridButtons((calcNumber % 4) * 8 + calcNumber / 4);
			}
		}
	}

}
