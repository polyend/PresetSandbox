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

#include <presetFlash.h>
#include "FastCRC.h"

#define USER_DATA_START_PAGE 6144U
#define USER_DATA_END_PAGE 	 8000U
#define CV_TABLE_PAGE  		 8050U

PresetFlash flash;

void PresetFlash::init()
{
	flash.init(FLASH_SPI_CS, SPI_MISO, SPI_MOSI, SPI_SCK);
}

void PresetFlash::readCalibrationData(CalibrationData *data)
{
	flash.readStructfromFlash(CV_TABLE_PAGE, (uint8_t*)data, sizeof(*data));
}

void PresetFlash::readUserData(void *data, size_t len, uint16_t startPage)
{
	flash.readStructfromFlash(USER_DATA_START_PAGE + startPage, (uint8_t*)data, len);
}

void PresetFlash::writeUserData(void *data, size_t len, uint16_t startPage)
{
	flash.writeStructToFlash(USER_DATA_START_PAGE + startPage, (uint8_t*)data, len);
}
