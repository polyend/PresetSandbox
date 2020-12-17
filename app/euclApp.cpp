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

#include "euclApp.h"

static constexpr int flashStartPage = 0;

void EuclideanApp::init()
{
	restoreFromFlash();
}

void EuclideanApp::update()
{
	if (gateTimer > gateOutTime)
	{
		for (int i = 0; i < generatorsCount; i++)
		{
			setCVOutVolt(generatorToOutIdx(i), cv_0V);
		}
	}
}

void EuclideanApp::onEncoderPress(int encoderIdx)
{
}

void EuclideanApp::onEncoderRelease(int encoderIdx)
{
}

void EuclideanApp::onEncoderRotate(int encoderIdx, int delta)
{
	int functionIdx;
	Generator &generator = generators[outToGeneratorIdx(encoderIdx, functionIdx)];

	switch (functionIdx)
	{
	case 0:
		generator.length = max(generator.length + delta, 1);
		break;
	case 1:
		generator.events = constrain(generator.events + delta, 1,
										generator.length);
		break;
	case 2:
		generator.shift = generator.shift - delta;
		break;
	}

	drawEvents();
	isDirty = true;
}

void EuclideanApp::onCVInputChange(int value)
{
	if (isInputHigh && value < cv_10V / 20)
	{
		isInputHigh = false;
	}
	if (!isInputHigh && value > cv_10V * 2 / 10)
	{
		isInputHigh = true;
		handleClock();
		gateTimer = 0;
		drawEvents();
		if (isDirty && generators[0].getCurrentStep() == 0)
		{
			saveToFlash();
			isDirty = false;
		}
	}
}

void EuclideanApp::onButtonPress(Buttons button)
{
	if (button == Clear)
	{
		for (int i = 0; i < generatorsCount; i++)
		{
			generators[i].reset();
		}
	}
	else if (button == Rec)
	{
		pause ^= true;
	}
}

void EuclideanApp::onButtonRelease(Buttons button)
{
}

void EuclideanApp::onPadPress(int padIdx)
{
}

void EuclideanApp::onPadRelease(int padIdx)
{
}

void EuclideanApp::handleClock()
{
	if (pause)
	{
		return;
	}

	for (int i = 0; i < generatorsCount; i++)
	{
		bool isEvent = generators[i].handleClock();

		if (isEvent)
		{
			setCVOutVolt(generatorToOutIdx(i), gateHighVolt);
		}
	}
}

void EuclideanApp::drawEvents()
{
	for (int genIdx = 0; genIdx < generatorsCount; genIdx++)
	{
		Generator &gen = generators[genIdx];
		const int page = gen.getCurrentStep() / padColumns;
		const int pageStart = page * padColumns;

		for (int step = pageStart; step < pageStart + padColumns; step++)
		{
			int intensity = 0;

			if (step < gen.length)
			{
				intensity =
						(!pause && gen.getCurrentStep() == step) ? maxIntensity * 2 / 3:
								(gen.isEventOnStep(step) ? maxIntensity : maxIntensity / 4);
			}

			setPadLED(genIdx * padColumns + (step - pageStart), intensity);
		}
	}
}

void EuclideanApp::saveToFlash()
{
	writeToFlash(generators, sizeof(generators), flashStartPage);
}

void EuclideanApp::restoreFromFlash()
{
	readFromFlash(generators, sizeof(generators), flashStartPage);

	for (int i = 0; i < generatorsCount; i++)
	{
		generators[i].validate();
		generators[i].reset();
	}
}

inline int EuclideanApp::generatorToOutIdx(int generatorIdx)
{
	return generatorIdx * 3;
}

inline int EuclideanApp::outToGeneratorIdx(int outIdx, int &remainderIdx)
{
	remainderIdx = outIdx % 3;
	return outIdx / 3;
}

bool EuclideanApp::Generator::isEventOnStep(int step)
{
	int shiftedStep = (step + shift) % length;
	float euclStepSize = (float) length / events;
	float nearestStep = round(shiftedStep / euclStepSize) * euclStepSize;
	float diff = nearestStep - (float) shiftedStep;
	return diff <= 0.5f && diff > -0.5f;
}

bool EuclideanApp::Generator::handleClock()
{
	step++;
	step %= length;

	return isEventOnStep(step);
}

void EuclideanApp::Generator::validate()
{
	if (length < 0 || events > length)
	{
		*this = Generator();
	}
}
