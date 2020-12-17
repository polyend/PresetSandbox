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

#ifndef DRIVERS_PRESETFLASH_H_
#define DRIVERS_PRESETFLASH_H_

#include "Arduino.h"
#include "Sodaq_dataflash.h"
#include "presetHardware.h"

class PresetFlash
{
public:
	typedef struct
	{
		typedef struct
		{
			uint16_t cv_tab[9][121];
		} CvTab;

		CvTab v_octave;
		CvTab hz_v;
	} CalibrationData;

	void init();

	void readCalibrationData(CalibrationData *data);

	/**
	 * Write any user data to flash area dedicated to this purpose,
	 * that does not overlap with areas used by baseline Preset firmware.
	 * Page size is 512 bytes. 1856 pages is available.
	 * Len does not have to be aligned to page size.
	 */
	void writeUserData(void *data, size_t len, uint16_t startPage);

	/**
	 * Read user data from flash.
	 */
	void readUserData(void *data, size_t len, uint16_t startPage);

private:
	Sodaq_Dataflash flash;
};

extern PresetFlash flash;

#endif
