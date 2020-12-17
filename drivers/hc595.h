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

#ifndef DRIVERS_HC595_H_
#define DRIVERS_HC595_H_

#include "Arduino.h"

class ShiftRegisterHC595
{
public:
	void init(uint8_t clock_pin, uint8_t data_pin, uint8_t cs_pin);
	void setMeasureFunc(void (*func)(uint8_t));
	void update();

private:
	void (*measureAction)(uint8_t);
	void shift(uint8_t data_bit);
	void clearRegister();
	uint8_t _clock_pin;
	uint8_t _data_pin;
	uint8_t _cs_pin;
};

extern ShiftRegisterHC595 shiftRegisterHC595;

#endif /* DRIVERS_HC595_H_ */
