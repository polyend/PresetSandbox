/*
 * Copyright (c) 2013 Kees Bakker.  All rights reserved.
 *
 * This file is part of Sodaq_dataflash.
 *
 * Sodaq_dataflash is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published bythe Free Software Foundation, either version 3 of
 * the License, or(at your option) any later version.
 *
 * Sodaq_dataflash is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Sodaq_dataflash.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/*
 * This library was inspired by dataflash libraries by Atmel, asynclabs,
 * and others.
 */

#include <stdint.h>
#include <Arduino.h>
#include <SPI.h>

#include "Sodaq_dataflash.h"

#include "../../drivers/presetHardware.h"

//Dataflash commands
#define FlashPageRead           0xD2    // Main memory page read
#define StatusReg               0xD7    // Status register
#define ReadMfgID               0x9F    // Read Manufacturer and Device ID
#define PageErase               0x81    // Page erase
#define ReadSecReg              0x77    // Read Security Register

#define FlashToBuf1Transfer     0x53    // Main memory page to buffer 1 transfer
#define Buf1Read                0xD4    // Buffer 1 read
#define Buf1ToFlashWE           0x83    // Buffer 1 to main memory page program with built-in erase
#define Buf1ToFlashWOE          0x88    // Buffer 1 to main memory page program without built-in erase
#define Buf1Write               0x84    // Buffer 1 write

#define AutoPageRewriteBuf1		0x58


#define FlashToBuf2Transfer     0x55    // Main memory page to buffer 2 transfer
#define Buf2Read                0xD6    // Buffer 2 read
#define Buf2ToFlashWE           0x86    // Buffer 2 to main memory page program with built-in erase
#define Buf2Write               0x87    // Buffer 2 write


//Sodaq_Dataflash flash;

SPISettings settingsSPI(40000000, MSBFIRST, SPI_MODE0);

//uint8_t _csPin, _misoPin, _mosiPin, _sckPin;
void Sodaq_Dataflash::init(uint8_t csPin, uint8_t misoPin, uint8_t mosiPin, uint8_t sckPin)
{
	// pin config
	_csPin   = csPin;
	_misoPin = misoPin;
	_mosiPin = mosiPin;
	_sckPin  = sckPin;

	pinMode(_csPin, OUTPUT);
	digitalWrite(_csPin, HIGH);


	delay(100);

	// Call the standard SPI initialisation
	// init SPI

	// SPI.setClockDivider(SPI_CLOCK_DIV2);

	SPI.setMOSI(_mosiPin);
	SPI.setMISO(_misoPin);
	SPI.setSCK(_sckPin);

	SPI.begin();

	// koniec initu SPI


#if DF_VARIANT == DF_AT45DB081D
	_pageAddrShift = 1;
#elif DF_VARIANT == DF_AT45DB161D
	_pageAddrShift = 1;
#elif DF_VARIANT == DF_AT45DB041D
	_pageAddrShift = 1;
#endif
}



uint8_t Sodaq_Dataflash::transmit(uint8_t data)
{
	// Call the standard SPI transfer method
	return SPI.transfer(data);
}




uint8_t Sodaq_Dataflash::readStatus()
{
	unsigned char result;

	activate();
	result = transmit(StatusReg);
	result = transmit(0x00);
	deactivate();

	return result;
}

// Monitor the status register, wait until busy-flag is high
void Sodaq_Dataflash::waitTillReady()
{
	while (!(readStatus() & 0x80)) {
		// WDT reset maybe??
	}
}

bool Sodaq_Dataflash::isReady(void)
{
	return (readStatus() & 0x80);
}

void Sodaq_Dataflash::readID(uint8_t *data)
{
	activate();
	transmit(ReadMfgID);
	data[0] = transmit(0x00);
	data[1] = transmit(0x00);
	data[2] = transmit(0x00);
	data[3] = transmit(0x00);
	deactivate();
}


// Reads a number of bytes from one of the Dataflash security register
void Sodaq_Dataflash::readSecurityReg(uint8_t *data, size_t size)
{
	activate();
	transmit(ReadSecReg);
	transmit(0x00);
	transmit(0x00);
	transmit(0x00);
	for (size_t i = 0; i < size; i++) {
		*data++ = transmit(0x00);
	}
	deactivate();
}

// Transfers a page from flash to Dataflash SRAM buffer
void Sodaq_Dataflash::readPageToBuf1(uint16_t pageAddr)
{
	activate();
	transmit(FlashToBuf1Transfer);
	setPageAddr(pageAddr);
	deactivate();

	waitTillReady();

	// Serial.print("read, waitTillReady: ");
	// Serial.println(timer);
}

// Transfers a page from flash to Dataflash SRAM buffer
void Sodaq_Dataflash::readPageToBuf2(uint16_t pageAddr)
{
	activate();
	transmit(FlashToBuf2Transfer);
	setPageAddr(pageAddr);
	deactivate();

	waitTillReady();

	// Serial.print("read, waitTillReady: ");
	// Serial.println(timer);
}

// Reads one byte from one of the Dataflash internal SRAM buffer 1
uint8_t Sodaq_Dataflash::readByteBuf1(uint16_t addr)
{
	unsigned char data = 0;

	activate();
	transmit(Buf1Read);
	transmit(0x00);               //don't care
	transmit((uint8_t) (addr >> 8));
	transmit((uint8_t) (addr));
	transmit(0x00);               //don't care
	data = transmit(0x00);        //read byte
	deactivate();

	return data;
}

uint8_t Sodaq_Dataflash::writeInsidePage(uint16_t bufferaddress, uint16_t pageaddr, uint8_t *data, size_t data_count)
{

	if(bufferaddress>DF_PAGE_SIZE || pageaddr>8191)
	{
		return 0;
	}

	if(data_count > (DF_PAGE_SIZE-bufferaddress))// check if the data will fit
	{
		if(!(data_count == DF_PAGE_SIZE && bufferaddress==0))
		{
			return 0;
		}

	}

	Sodaq_Dataflash::readPageToBuf1(pageaddr);//move from flash to buffer

	activate();
	delayMicroseconds(5);
	transmit(Buf1Write);
	transmit(0x00);//don't care
	transmit((bufferaddress>>8 & 0x01));//buffer adress
	transmit(bufferaddress);//buffer adress

	for (uint16_t i = 0; i <data_count; i++)
	{
		transmit(*data++);
	}

	deactivate();

	Sodaq_Dataflash::writeBuf1ToPage(pageaddr);//move from buffer to flash

	waitTillReady();

	return 1;
}


void Sodaq_Dataflash::readPagefromFlash(uint16_t buffaddress,uint16_t pageaddr, uint8_t *data, uint16_t size)
{
	if(buffaddress > 512 || pageaddr >8191)
	{
		return;
	}

	Sodaq_Dataflash::readPageToBuf1(pageaddr);

	activate();
	transmit(Buf1Read);
	transmit(0x00);//don't care
	transmit((buffaddress>>8 & 0x01));//buffer adress
	transmit(buffaddress);//buffer adress
	transmit(0x00);


	for (uint16_t i = 0; i < size; i++)
	{
		*data++ = transmit(0x00);
	}

	deactivate();

	waitTillReady();
}

// Reads structure directly from flash, buffering included
void Sodaq_Dataflash::readStructfromFlash(uint16_t addr, uint8_t *data, uint16_t size)
{
	int16_t temp;

	for(temp=size;temp>0;temp=temp-DF_PAGE_SIZE)
	{
		Sodaq_Dataflash::readPageToBuf1(addr);//move from flash to buffer

		activate();
		transmit(Buf1Read);
		transmit(0x00);//don't care
		transmit(0x00);//buffer adress
		transmit(0x00);//buffer adress
		transmit(0x00);

		if(temp>(signed)DF_PAGE_SIZE)
		{
			for (uint16_t i = 0; i < DF_PAGE_SIZE; i++)
			{
				*data++ = transmit(0x00);
			}
			addr++;
		}
		else
		{
			for (uint16_t i = 0; i < temp; i++)
			{
				*data++ = transmit(0x00);
			}
		}

		deactivate();

	}
	waitTillReady();
}

// Writes one byte to one to the Dataflash internal SRAM buffer 1
void Sodaq_Dataflash::writeByteBuf1(uint16_t addr, uint8_t data)
{
	activate();
	transmit(Buf1Write);
	transmit(0x00);               //don't care
	transmit((uint8_t) (addr >> 8));
	transmit((uint8_t) (addr));
	transmit(data);               //write data byte
	deactivate();
}


// Writes structure directly to flash, buffering included
void Sodaq_Dataflash::writeStructToFlash(uint16_t addr, uint8_t *data, uint16_t size)
{
	volatile int16_t temp;

	for(temp=size;temp>0;temp=temp-DF_PAGE_SIZE)
	{
		activate();
		delayMicroseconds(5);
		transmit(Buf1Write);
		transmit(0x00);//don't care
		transmit(0);//buffer adress
		transmit(0);//buffer adress

		if(temp>(signed)DF_PAGE_SIZE)
		{
			for (uint16_t i = 0; i <DF_PAGE_SIZE; i++)
			{
				transmit(*data++);
			}

		}
		else
		{

			for (uint16_t i = 0; i < temp; i++)
			{
				transmit(*data++);
			}
		}
		deactivate();
		Sodaq_Dataflash::writeBuf1ToPage(addr);//move from buffer to flash
		addr++;
	}

	waitTillReady();
}


// Transfers Dataflash SRAM buffer 1 to flash page
void Sodaq_Dataflash::writeBuf1ToPage(uint16_t pageAddr)
{

	activate();
	transmit(Buf1ToFlashWE);
	setPageAddr(pageAddr);
	deactivate();

	// elapsedMicros timer = 0;
	waitTillReady();
	// Serial.print("write, waitTillReady: ");
	// Serial.println(timer);
}

// Writes a number of bytes to one of the Dataflash internal SRAM buffer 1
void Sodaq_Dataflash::writeStrBuf2(uint16_t addr, uint8_t *data, size_t size)
{
	// elapsedMicros timer = 0;
	activate();
	delayMicroseconds(5);
	transmit(Buf2Write);
	transmit(0x00);               //don't care
	transmit((uint8_t) (addr >> 8));
	transmit((uint8_t) (addr));
	for (size_t i = 0; i < size; i++) {
		transmit(*data++);
	}
	deactivate();
	// Serial.print("writeStrBuf2: ");
	// Serial.println(timer);
}

// Transfers Dataflash SRAM buffer 1 to flash page
void Sodaq_Dataflash::writeBuf2ToPage(uint16_t pageAddr)
{

	activate();
	transmit(Buf2ToFlashWE);
	setPageAddr(pageAddr);
	deactivate();

	// elapsedMicros timer = 0;
	waitTillReady();
	// Serial.print("write, waitTillReady: ");
	// Serial.println(timer);
}

void Sodaq_Dataflash::pageErase(uint16_t pageAddr)
{
	activate();
	transmit(PageErase);
	setPageAddr(pageAddr);
	deactivate();
	waitTillReady();
}

void Sodaq_Dataflash::chipErase()
{
	activate();
	transmit(0xC7);
	transmit(0x94);
	transmit(0x80);
	transmit(0x9A);
	deactivate();
	waitTillReady();
}

void Sodaq_Dataflash::settings(SPISettings settings)
{
	_settings = settings;
}

void Sodaq_Dataflash::deactivate()
{
	digitalWrite(_csPin, HIGH);
	SPI.endTransaction();
}



void Sodaq_Dataflash::activate()
{
	SPI.beginTransaction(settingsSPI);
	digitalWrite(_csPin, LOW);
}

void Sodaq_Dataflash::setPageAddr(unsigned int pageAddr)
{
	transmit(getPageAddrByte0(pageAddr));
	transmit(getPageAddrByte1(pageAddr));
	transmit(getPageAddrByte2(pageAddr));
}

/*
 * From the AT45DB081D documentation (other variants are not really identical)
 *   "For the DataFlash standard page size (264-bytes), the opcode must be
 *    followed by three address bytes consist of three don’t care bits,
 *    12 page address bits (PA11 - PA0) that specify the page in the main
 *    memory to be written and nine don’t care bits."
 */
/*
 * From the AT45DB161B documentation
 *   "For the standard DataFlash page size (528 bytes), the opcode must be
 *    followed by three address bytes consist of 2 don’t care bits, 12 page
 *    address bits (PA11 - PA0) that specify the page in the main memory to
 *    be written and 10 don’t care bits."
 */
/*
 * From the AT45DB041D documentation
 *   "For the DataFlash standard page size (264-bytes), the opcode must be
 *   followed by three address bytes consist of four don’t care bits, 11 page
 *   address bits (PA10 - PA0) that specify the page in the main memory to
 *   be written and nine don’t care bits."
 */
uint8_t Sodaq_Dataflash::getPageAddrByte0(uint16_t pageAddr)
{
	// More correct would be to use a 24 bits number
	// shift to the left by number of bits. But the uint16_t can be considered
	// as if it was already shifted by 8.
	return (pageAddr << (DF_PAGE_BITS - 8)) >> 8;
}
uint8_t Sodaq_Dataflash::getPageAddrByte1(uint16_t page)
{
	return page << (DF_PAGE_BITS - 8);
}
uint8_t Sodaq_Dataflash::getPageAddrByte2(uint16_t page)
{
	return 0;
}

