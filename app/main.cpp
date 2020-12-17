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
#include <presetFlash.h>
#include "Arduino.h"
#include "Sodaq_dataflash.h"

#include "presetEncoder.h"
#include "presetDAC.h"
#include "mtLED.h"
#include "presetButtons.h"
#include "presetMultiplex.h"
#include "presetSwitches.h"

#include "euclApp.h"

#define APP_CLASS EuclideanApp

APP_CLASS application;
PresetDAC dac;

void onEncoderPress(uint8_t n)
{
	application.onEncoderPress(n);
}
void onEncoderRelease(uint8_t n)
{
	application.onEncoderRelease(n);
}

void onEncoderRotate(uint8_t enc_number,uint8_t direction)
{
	application.onEncoderRotate(enc_number,direction == LEFT ? -1 : 1);
}

void onCVin(uint16_t value)
{
	application.onCVInputChange(value);
}

void onButtonPress(uint8_t n)
{
	application.onButtonPress(PresetApp::Buttons(n));
}

void onButtonRelease(uint8_t n)
{
	application.onButtonRelease(PresetApp::Buttons(n));
}

void onPadPress(uint8_t n)
{
	application.onPadPress(n);
}
void onPadRelease(uint8_t n)
{
	application.onPadRelease(n);
}

void presetHardwareInit()
{
	Serial.begin(9600);

	delay(500);

	flash.init();

	dac.init();

	presetLeds.init(IS31FL3731_ADDR1);

	initMultiplex();
	encoderSwitches.setPressFunction(onEncoderPress);
	encoderSwitches.setReleaseFunction(onEncoderRelease);
	tactSwitches.setPressFunction(onButtonPress);
	tactSwitches.setReleaseFunction(onButtonRelease);
	setPressActionGridButtons(onPadPress);
	setReleaseActionGridButtons(onPadRelease);
	encoderSwitches.init();
	tactSwitches.init();
	initGridButtons();

	dac.setCVinHandler(onCVin);

	initEncoders(onEncoderRotate);
}

void presetHardwareUpdate()
{
	encoderSwitches.update();
	tactSwitches.update();
	updateGridButtons();

	presetLeds.checkFault();
	presetLeds.update();

	dac.update();
}

void setup()
{
	presetHardwareInit();
	application.init();
}

void loop()
{
	presetHardwareUpdate();
	application.update();
}
