#ifndef SODAQ_DATAFLASH_H
#define SODAQ_DATAFLASH_H
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

#include <stddef.h>
#include <stdint.h>
#include <SPI.h>

#define DF_AT45DB081D   1
#define DF_AT45DB161D   2
#define DF_AT45DB041D   3
#define DF_AT45DB321E   4

#ifndef DF_VARIANT
#define DF_VARIANT DF_AT45DB321E
#endif

#if DF_VARIANT == DF_AT45DB081D
// configuration for the Atmel AT45DB081D device, Sodaq v2 has AT45DB081D, see doc3596.pdf, 4096 pages of 256/264 bytes
#define DF_PAGE_ADDR_BITS       12
#define DF_PAGE_SIZE            264
#define DF_PAGE_BITS            9
/*
 * From the AT45DB081D documentation (other variants are not really identical)
 *   "For the DataFlash standard page size (264-bytes), the opcode must be
 *    followed by three address bytes consist of three don’t care bits,
 *    12 page address bits (PA11 - PA0) that specify the page in the main
 *    memory to be written and nine don’t care bits."
 */

#elif DF_VARIANT == DF_AT45DB161D
// configuration for the Atmel AT45DB161D device
#define DF_PAGE_ADDR_BITS       12
#define DF_PAGE_SIZE            528
#define DF_PAGE_BITS            10
/*
 * From the AT45DB161B documentation
 *   "For the standard DataFlash page size (528 bytes), the opcode must be
 *    followed by three address bytes consist of 2 don’t care bits, 12 page
 *    address bits (PA11 - PA0) that specify the page in the main memory to
 *    be written and 10 don’t care bits."
 */

#elif DF_VARIANT == DF_AT45DB041D
// configuration for the Atmel AT45DB041D device
#define DF_PAGE_ADDR_BITS       11
#define DF_PAGE_SIZE            264
#define DF_PAGE_BITS            9
/*
 * From AT45DB041D documentation
 *   "For the DataFlash standard page size (264-bytes), the opcode must be
 *   followed by three address bytes consist of four don’t care bits, 11 page
 *   address bits (PA10 - PA0) that specify the page in the main memory to
 *   be written and nine don’t care bits."
 */



#elif DF_VARIANT == DF_AT45DB321E
// configuration for the Atmel AT45DB32
#define DF_PAGE_ADDR_BITS       13
#define DF_PAGE_SIZE            512U
#define DF_PAGE_BITS            10

#elif DF_VARIANT  == DF_AT45DB021E

#define DF_PAGE_ADDR_BITS 10
#define DF_PAGE_SIZE 256
#define DF_PAGE_BITS 9

/*
 * From the AT45DB161B documentation
 *   "For the standard DataFlash page size (528 bytes), the opcode must be
 *    followed by three address bytes consist of 2 don’t care bits, 12 page
 *    address bits (PA11 - PA0) that specify the page in the main memory to
 *    be written and 10 don’t care bits."
 */

#else
#error "Unknown DF_VARIANT"
#endif
#define DF_NR_PAGES     (1 << DF_PAGE_ADDR_BITS)



class Sodaq_Dataflash
{
public:
  // void init(uint8_t csPin=SS);
  void init(uint8_t csPin, uint8_t misoPin, uint8_t mosiPin, uint8_t sckPin);
  void readID(uint8_t *data);
  void readSecurityReg(uint8_t *data, size_t size);

  uint8_t readByteBuf1(uint16_t pageAddr);
  void readStructfromFlash(uint16_t addr, uint8_t *data, uint16_t size);

  void writeByteBuf1(uint16_t addr, uint8_t data);
  void writeStructToFlash(uint16_t addr, uint8_t *data, uint16_t size);
  void writeByteBuf2(uint16_t addr, uint8_t data);
  void writeStrBuf2(uint16_t addr, uint8_t *data, size_t size);

  void writeBuf1ToPage(uint16_t pageAddr);
  void writeBuf2ToPage(uint16_t pageAddr);

  void readPageToBuf1(uint16_t pageAddr);
  void readPageToBuf2(uint16_t pageAddr);

  void pageErase(uint16_t pageAddr);
  void chipErase();

  void settings(SPISettings settings);
  bool isReady(void);
  void waitTillReady();

  uint8_t writeInsidePage(uint16_t bufferaddress, uint16_t addr, uint8_t *data, size_t data_count);
  void readPagefromFlash(uint16_t buffaddress,uint16_t pageaddr, uint8_t *data, uint16_t size);

private:

  uint8_t readStatus();
  uint8_t transmit(uint8_t data);
  void activate();
  void deactivate();
  void setPageAddr(unsigned int PageAdr);
  uint8_t getPageAddrByte0(uint16_t pageAddr);
  uint8_t getPageAddrByte1(uint16_t pageAddr);
  uint8_t getPageAddrByte2(uint16_t pageAddr);

  uint8_t _csPin, _misoPin, _mosiPin, _sckPin;

  size_t _pageAddrShift;
  SPISettings _settings;
};


//extern Sodaq_Dataflash flash;




#endif // SODAQ_DATAFLASH_H
