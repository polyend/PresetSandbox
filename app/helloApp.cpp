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

#include "helloApp.h"

constexpr int encoderStep = 64;

void HelloApp::init()
{
	for (int i = 0; i < outputCount; i++)
	{
		outs[i] = 0;
	}
}

void HelloApp::update()
{
	for (int i = 0; i < outputCount; i++)
	{
		setCVOutVolt(i, outs[i]);
		setEncoderLED(i, Blue, outs[i] * maxIntensity / cv_10V);
	}
}

void HelloApp::onEncoderPress(int encoderIdx)
{
	outs[encoderIdx] = cv_10V;
}

void HelloApp::onEncoderRelease(int encoderIdx)
{
	outs[encoderIdx] = cv_0V;
}

void HelloApp::onEncoderRotate(int encoderIdx, int delta)
{
	outs[encoderIdx] += delta * encoderStep;

	if (outs[encoderIdx] > cv_10V)
	{
		outs[encoderIdx] = cv_10V;
	}
	else if (outs[encoderIdx] < cv_0V)
	{
		outs[encoderIdx] = cv_0V;
	}
}

void HelloApp::onCVInputChange(int value)
{
	setPadLED(0, value * maxIntensity / cv_10V);
}

void HelloApp::onButtonPress(Buttons button)
{
}

void HelloApp::onButtonRelease(Buttons button)
{
}

void HelloApp::onPadPress(int padIdx)
{
	setPadLED(padIdx, maxIntensity);
}

void HelloApp::onPadRelease(int padIdx)
{
	setPadLED(padIdx, 0);
}
