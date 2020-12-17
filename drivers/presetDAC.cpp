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
#include "presetDAC.h"
#include "presetHardware.h"

static const float speedMultiplier[256] =
		{
				0.0639, 0.0653, 0.0667, 0.0682, 0.0696, 0.0712, 0.0727, 0.0743, 0.0760, 0.0776, 0.0793, 0.0811, 0.0828, 0.0846, 0.0865, 0.0884,
				0.0903, 0.0923, 0.0943, 0.0964, 0.0985, 0.1007, 0.1029, 0.1051, 0.1074, 0.1098, 0.1122, 0.1146, 0.1171, 0.1197, 0.1223, 0.1250,
				0.1277, 0.1305, 0.1334, 0.1363, 0.1393, 0.1423, 0.1455, 0.1487, 0.1519, 0.1552, 0.1586, 0.1621, 0.1657, 0.1693, 0.1730, 0.1768,
				0.1806, 0.1846, 0.1886, 0.1928, 0.1970, 0.2013, 0.2057, 0.2102, 0.2148, 0.2195, 0.2243, 0.2293, 0.2343, 0.2394, 0.2446, 0.2500,
				0.2555, 0.2611, 0.2668, 0.2726, 0.2786, 0.2847, 0.2909, 0.2973, 0.3038, 0.3105, 0.3173, 0.3242, 0.3313, 0.3386, 0.3460, 0.3536,
				0.3613, 0.3692, 0.3773, 0.3856, 0.3940, 0.4026, 0.4114, 0.4204, 0.4297, 0.4391, 0.4487, 0.4585, 0.4685, 0.4788, 0.4893, 0.5000,
				0.5109, 0.5221, 0.5336, 0.5453, 0.5572, 0.5694, 0.5819, 0.5946, 0.6076, 0.6209, 0.6345, 0.6484, 0.6626, 0.6771, 0.6920, 0.7071,
				0.7226, 0.7384, 0.7546, 0.7711, 0.7880, 0.8052, 0.8229, 0.8409, 0.8593, 0.8781, 0.8974, 0.9170, 0.9371, 0.9576, 0.9786, 1.0000,
				1.0219, 1.0443, 1.0671, 1.0905, 1.1144, 1.1388, 1.1637, 1.1892, 1.2152, 1.2419, 1.2691, 1.2968, 1.3252, 1.3543, 1.3839, 1.4142,
				1.4452, 1.4768, 1.5092, 1.5422, 1.5760, 1.6105, 1.6458, 1.6818, 1.7186, 1.7563, 1.7947, 1.8340, 1.8742, 1.9152, 1.9571, 2.0000,
				2.0438, 2.0885, 2.1343, 2.1810, 2.2288, 2.2776, 2.3274, 2.3784, 2.4305, 2.4837, 2.5381, 2.5937, 2.6505, 2.7085, 2.7678, 2.8284,
				2.8904, 2.9537, 3.0183, 3.0844, 3.1520, 3.2210, 3.2915, 3.3636, 3.4372, 3.5125, 3.5894, 3.6680, 3.7483, 3.8304, 3.9143, 4.0000,
				4.0876, 4.1771, 4.2686, 4.3620, 4.4575, 4.5552, 4.6549, 4.7568, 4.8610, 4.9674, 5.0762, 5.1874, 5.3009, 5.4170, 5.5356, 5.6569,
				5.7807, 5.9073, 6.0367, 6.1688, 6.3039, 6.4420, 6.5830, 6.7272, 6.8745, 7.0250, 7.1788, 7.3360, 7.4967, 7.6608, 7.8286, 8.0000,
				8.1752, 8.3542, 8.5371, 8.7241, 8.9151, 9.1103, 9.3098, 9.5137, 9.7220, 9.9349, 10.1524, 10.3747, 10.6019, 10.8340, 11.0713,
				11.3137, 11.5614, 11.8146, 12.0733, 12.3377, 12.6078, 12.8839, 13.1660, 13.4543, 13.7490, 14.0500, 14.3577, 14.6721, 14.9933,
				15.3217, 15.6572, 16.0000
		};

void (*CVinHandler)(uint16_t number)=NULL;
void (*CVgateHandler)()=NULL;

static uint16_t prev_value;

static PresetFlash::CalibrationData calibration;

PresetDAC::PresetDAC()
{

}
PresetDAC::~PresetDAC()
{

}

void PresetDAC::init()
{
	max_init();
	flash.readCalibrationData(&calibration);
}

uint16_t PresetDAC::getCVvalue()
{
	uint16_t temp;
	if (max_isADCReady(0))
	{
		temp = max_readADC(0);
		prev_value = temp;

		return temp;
	}
	else
	{
		return prev_value;
	}
}

bool PresetDAC::update()
{
	uint16_t curr_value;

	if (max_isADCReady(0))
	{
		curr_value = max_readADC(0);

		prev_value = curr_value;
		if (CVinHandler != NULL)
		{
			(*CVinHandler)(curr_value);
		}
	}

	return 1;
}

void PresetDAC::setCVinHandler(void (*pntr)(uint16_t))
{
	CVinHandler = pntr;
}

void PresetDAC::setCVOutVolt(int outIdx, int value)
{
	max_writeDAC(value, outIdx);
}

void PresetDAC::setCVOutNote(int outIdx, int note, int detune)
{
	uint16_t noteDAC = calibration.v_octave.cv_tab[outIdx][note];
	uint16_t nextNoteDAC = calibration.v_octave.cv_tab[outIdx][note + 1];
	uint16_t diff = nextNoteDAC - noteDAC;
	float step = diff / 7.0;
	uint16_t detuneDAC = round(step * detune);

	max_writeDAC(outIdx, noteDAC + detuneDAC);
}
