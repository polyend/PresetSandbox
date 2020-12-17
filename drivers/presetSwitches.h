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

#ifndef DRIVERS_PRESETSWITCHES_H_
#define DRIVERS_PRESETSWITCHES_H_

#include <stdint.h>
#include "elapsedMillis.h"

class presetEncoderSwitches
{
public:
	void init();
	void update();
	void setPressFunction(void (*func)(uint8_t n));
	void setReleaseFunction(void (*func)(uint8_t n));

private:
	elapsedMillis offTimer[9];
	uint8_t lastState[9];
	uint8_t currentState[9];
	uint8_t offMeasureFlag[9];
	uint8_t madePressFlag[9];
	uint8_t madeReleaseFlag[9];

	void (*pressAction)(uint8_t n);
	void (*releaseAction)(uint8_t n);
};

class presetTactSwitches
{
public:
	void init();
	void update();
	void setPressFunction(void (*func)(uint8_t n));
	void setReleaseFunction(void (*func)(uint8_t n));

private:
	elapsedMillis offTimer[3];
	uint8_t currentState[3];
	uint8_t offMeasureFlag[3];
	uint8_t madePressFlag[3];
	uint8_t madeReleaseFlag[3];

	void (*pressAction)(uint8_t n);
	void (*releaseAction)(uint8_t n);
};

extern presetEncoderSwitches encoderSwitches;
extern presetTactSwitches tactSwitches;

#endif /* DRIVERS_PRESETSWITCHES_H_ */
