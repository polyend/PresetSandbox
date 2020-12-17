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

#ifndef APP_PRESETAPP_H_
#define APP_PRESETAPP_H_

#include <stdint.h>
#include <stddef.h>

// Base class for application (functional) code.
// Public section contains methods that can be overriden to handle events from hardware.
// Protected section defines API for accessing the hardware.
//
// The final class of application to be executed is defined as APP_CLASS in main.cpp.
class PresetApp
{
public:
	enum Buttons
	{
		Clear, Rec, Bank
	};

	enum EncoderLedColors
	{
		Off, Green, Blue, Cyan
	};

	// Maximum LED intensity
	static constexpr int maxIntensity = 100;

	// Mapping of in/out voltage levels
	static constexpr int cv_0V = 0;
	static constexpr int cv_10V = 4095;
	static constexpr int cv_1V = cv_10V / 10;

	static constexpr int encoderCount = 9;
	static constexpr int outputCount = 9;
	static constexpr int padRows = 4;
	static constexpr int padColumns = 8;
	static constexpr int padCount = padRows * padColumns;

	// Initialize the application, called once on power on, after all hardware is ready.
	void init();
	
	// Update the application state, called indifinitely from loop in main. 
	// The function should not execute too long, otherwise updates to hardware will be delayed.
	void update();

	// Notifications on hardware events.
	void onEncoderPress(int encoderIdx);
	void onEncoderRelease(int encoderIdx);
	void onEncoderRotate(int encoderIdx, int delta);
	void onCVInputChange(int value);
	void onButtonPress(Buttons button);
	void onButtonRelease(Buttons button);
	void onPadPress(int padIdx);
	void onPadRelease(int padIdx);

protected:
	// Set color and intensity of encoder LED
	void setEncoderLED(int encoderIdx, EncoderLedColors color, int intensity);
	
	// Set intensity of pad LED
	void setPadLED(int padIdx, int intensity);
	
	// Draw a vizualization of the value on all pads,
	// e.g. for value 1 only the first pad is lit, for value 48 all the pads are on.
	void showValueOnPads(int value);

	// Get current voltage of the input.
	int getCVInput();
	
	// Set voltage on output and keep it until further change.
	void setCVOutVolt(int outIdx, int voltage);
	
	// Set voltage on output mapped from note value, using factory calibration data.
	// note - index by semitones, maps to 1V/octane (12 semitones). 0 is 0V.
	void setCVOutNote(int outIdx, int note, int detune);

	
	// Write any user data to flash area dedicated to this purpose,
	// that does not overlap with areas used by baseline Preset firmware.
	// Page size is 512 bytes. 1856 pages is available.
	// Len does not have to be aligned to page size.
	void writeToFlash(void *data, size_t len, uint16_t startPage);

	// Read user data from flash.
	void readFromFlash(void *data, size_t len, uint16_t startPage);
};

#endif /* APP_PRESETAPP_H_ */
