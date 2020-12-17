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

#ifndef APP_EUCLAPP_H_
#define APP_EUCLAPP_H_

#include "presetApp.h"

// Basic example of an application.
// Functionality: 
// Provide constant voltage on all outputs, modify the value with encoders. 
// Encoders are lit blue with intensity corresponding to the value set.
// Press encoder to provide immediate 10V, release for 0V.
// Pad 1 intensity reflects voltage on the input.
// All pads light up when pressed.
class HelloApp: public PresetApp
{
private:
	int outs[outputCount];

public:
	void init();
	void update();

	void onEncoderPress(int encoderIdx);
	void onEncoderRelease(int encoderIdx);
	void onEncoderRotate(int encoderIdx, int delta);
	void onCVInputChange(int value);
	void onButtonPress(Buttons button);
	void onButtonRelease(Buttons button);
	void onPadPress(int padIdx);
	void onPadRelease(int padIdx);
};

#endif /* APP_EUCLAPP_H_ */
