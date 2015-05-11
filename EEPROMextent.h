/*
EEPROMextent.h - EEPROMextent library
Copyright (c) 2015 Thierry Paris.  All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef __EEPROMextent_h__
#define __EEPROMextent_h__

#ifndef VISUALSTUDIO
#include <avr/eeprom.h>
#include <inttypes.h>
#endif

#include "arduino.h"

#ifdef VISUALSTUDIO
uint8_t eeprom_read_byte(uint8_t * address);
void eeprom_write_byte(uint8_t * address, uint8_t value);
void eeprom_update_byte(uint8_t * address, uint8_t value);

void eeprom_read_block(uint8_t *dst, const uint8_t *src, size_t inSize);
void eeprom_write_block(const uint8_t *src, uint8_t *dst, size_t inSize);
void eeprom_update_block(const uint8_t *src, uint8_t *dst, size_t inSize);
#endif

////////////////////////////////////////////////////////
// Add a '//' at the beginning of the line to be in 
// release mode.
//#define DEBUG_MODE

class EEPROMextentClass
{
public:
	inline uint8_t read(int address) { return eeprom_read_byte((uint8_t *)address); }
	inline void write(int address, uint8_t value) { eeprom_write_byte((uint8_t *)address, value); }
	inline void update(int address, uint8_t value) { eeprom_update_byte((uint8_t *)address, value); }

#ifdef VISUALSTUDIO
	EEPROMextentClass();
#endif

	template <class T> int readAnything(int ee, T& value)
	{
		eeprom_read_block((byte*)(void*)&value, (const uint8_t *)ee, sizeof(value));
		return sizeof(value);
	}

	template <class T> int writeAnything(int ee, const T& value)
	{
		eeprom_write_block((const byte*)(const void*)&value, (uint8_t *)ee, sizeof(value));
		return sizeof(value);
	}

	template <class T> int updateAnything(int ee, const T& value)
	{
		eeprom_update_block((const byte*)(const void*)&value, (uint8_t *)ee, sizeof(value));
		return sizeof(value);
	}

	char *readString(int inStartPos, char *outString, int inMaxLen);
	int writeString(int inStartPos, const char *inString);
	int updateString(int inStartPos, const char *inString);

	void clear(int inStartingAddress, int inSize, byte inFillCharacter = 0);
};

extern EEPROMextentClass EEPROMextent;

#include "EEPROM_ItemList.hpp"
#include "CircularBuffer.hpp"

#endif
