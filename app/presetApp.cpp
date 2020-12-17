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

#include "presetApp.h"

#include <presetFlash.h>
#include "../drivers/mtLED.h"
#include "../drivers/presetHardware.h"
#include "../drivers/presetDAC.h"

extern PresetDAC dac;

void PresetApp::init()
{

}

void PresetApp::update()
{

}

void PresetApp::onEncoderPress(int encoderIdx)
{

}

void PresetApp::onEncoderRelease(int encoderIdx)
{

}

void PresetApp::onEncoderRotate(int encoderIdx, int delta)
{

}

void PresetApp::onCVInputChange(int value)
{

}

void PresetApp::onButtonPress(Buttons button)
{

}

void PresetApp::onButtonRelease(Buttons button)
{

}

void PresetApp::onPadPress(int padIdx)
{

}

void PresetApp::onPadRelease(int padIdx)
{

}

void PresetApp::setEncoderLED(int encoderIdx, EncoderLedColors color,
								int intensity)
{
	static uint16_t colorsMap[] = { 0, GREEN, BLUE, CYAN1 };

	presetLeds.setEncoderColor(encoderIdx,
								intensity == 0 || color == Off ? 0 : 1,
								colorsMap[color], intensity);
}

void PresetApp::setPadLED(int padIdx, int intensity)
{
	presetLeds.setPresetGridLED(padIdx, intensity != 0,
								intensity * mtLED::maxGammaPwm / maxIntensity);
}

void PresetApp::showValueOnPads(int value)
{
	for (int i = 0; i < padCount; i++)
	{
		setPadLED(i, i < value ? maxIntensity / 2 : 0);
	}
}

int PresetApp::getCVInput()
{
	return dac.getCVvalue();
}

void PresetApp::setCVOutVolt(int outIdx, int voltage)
{
	dac.setCVOutVolt(outIdx, voltage);
}

void PresetApp::setCVOutNote(int outIdx, int note, int detune)
{
	dac.setCVOutNote(outIdx, note, detune);
}

void PresetApp::readFromFlash(void *data, size_t len, uint16_t startPage)
{
	flash.readUserData(data, len, startPage);
}

void PresetApp::writeToFlash(void *data, size_t len, uint16_t startPage)
{
	flash.writeUserData(data, len, startPage);
}

