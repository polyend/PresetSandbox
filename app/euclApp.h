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
#include "elapsedMillis.h"

// More complex example of a Preset application.
// Implements 3-track Euclidean sequencer.
// * Input receives clock signal.
// * Outputs 1, 4, 7 send trigger signals.
// * Encoders modify 3 parameters of each track (each row for the output on left):
//   sequence length, number of events, offset
// * Pads visualize sequences as they are played, switching pages for sequences longer than 8.
// * Button Rec pauses/resumes playback.
// * Button Clear resets playback in all channels (jump to step 0).
class EuclideanApp : public PresetApp
{
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

private:
	static constexpr int generatorsCount = 3;
	static constexpr int gateHighVolt = cv_1V * 5;
	static constexpr int gateOutTime = 10;

	class Generator
	{
	private:
		int step = 0;

	public:
		int length = 1;
		int events = 1;
		int shift = 0;

		bool handleClock();
		bool isEventOnStep(int step);
		int getCurrentStep() { return step; };
		void reset() { step = 0; };
		void validate();
	};

	bool isInputHigh;
	elapsedMillis gateTimer;
	Generator generators[generatorsCount];
	bool pause = false;
	bool isDirty = false;

	void handleClock();
	int generatorToOutIdx(int generatorIdx);
	int outToGeneratorIdx(int outIdx, int &remainderIdx);

	void drawEvents();

	void saveToFlash();
	void restoreFromFlash();
};


#endif /* APP_EUCLAPP_H_ */
