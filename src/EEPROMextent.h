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

/*! \mainpage

A full explanation in french can be read at http://www.locoduino.org/ecrire/?exec=article&action=redirect&type=article&id=139 .

Roughly, EEPROM is small memory area which have two problems : first, the basic functions emmbedded in the IDE to write/read
data in this memory is very basic, and second, its lifetime could be short (relatively) if you always write the same cell of
this memory. This library try to answers to these two issues.

1. A new bunch of functions has been added to write/read strings defined as chars array.
2. For old IDE versions two functions has been added to write/read anything in this memory. Since IDE version 1.6.2 these
functions are included in the IDE delivered EEPROM library.
3. A 'clear' function is added to reset a part of this memory to a specific char.
4. this library implements the ATMEL solution to increase lifetime memory. See http://www.atmel.com/images/doc2526.pdf 
5. A complex list of hierarchical and heterogeneous data can be saved with a class, using parent/child paradigm.

As for many of my libaries, a '#define EEPROMEXTENT_DEBUG_MODE' can be unremarked to have some debug informations
and new functions to understand bugs in your sketch.
*/

#ifndef __EEPROMextent_h__
#define __EEPROMextent_h__

#ifndef VISUALSTUDIO
#include <avr/eeprom.h>
#include <inttypes.h>
//#include <EEPROM.h>
#define INT64
#else
#include <EEPROM.h>
#define INT64	(__int64)
#endif

////////////////////////////////////////////////////////
// Remove the '//' at the beginning of the line to be in 
// debug mode.
//#define EEPROMEXTENT_DEBUG_MODE

/// Main class for basic functions.
class EEPROMextentClass
{
public:
	/**
		* Read one byte from the given address.
		* @param address EEPROM address to read.
		* @return byte read at this address.
		* @see writeByte()
		* @see updateByte()
     */
	inline uint8_t readByte(int address) { return eeprom_read_byte((uint8_t *)INT64 address); }
	/**
		* Write one byte to the given address.
		* @param address EEPROM address to write to.
		* @param value new value of this address.
		* @see readByte()
		* @see updateByte()
     */
	inline void writeByte(int address, uint8_t value) { eeprom_write_byte((uint8_t *)INT64 address, value); }
	/**
		* Update one byte at the given address. If the content of this address is already of this value, it does not write the byte.
	This function will be longer (read + write) if the byte have to be written, and faster (only read) if the content is already correct.
		* @param address EEPROM address to write to.
		* @param value new value of this address.
		* @see readByte()
		* @see updateByte()
     */
	inline void updateByte(int address, uint8_t value) { eeprom_update_byte((uint8_t *)INT64 address, value); }

#ifdef VISUALSTUDIO
	EEPROMextentClass();
#endif

	/**
		* Read one object starting from the given address. The size of the object is automatically computed.
		* @param address EEPROM address to read.
		* @param value object to fill with the bytes loaded from EEPROM memory.
		* @return Number of bytes loaded.
		* @see writeAnything()
		* @see updateAnything()
     */
	template <class T> int readAnything(int address, T& value)
	{
		eeprom_read_block((byte*)(void*)&value, (const uint8_t *)INT64 address, sizeof(value));
		return sizeof(value);
	}

	/**
		* Write one object starting to the given address. The size of the object is automatically computed.
		* @param address EEPROM address to begin to write to.
		* @param value object to store at this address, for the size (with sizeof()) of this object.
		* @return Number of bytes written.
		* @see readAnything()
		* @see updateAnything()
     */
	template <class T> int writeAnything(int address, const T& value)
	{
		eeprom_write_block((const byte*)(const void*)&value, (uint8_t *)INT64 address, sizeof(value));
		return sizeof(value);
	}

	/**
		* Update one object starting to the given address. The size of the object is automatically computed.
		Update each byte of the object starting at the given address.
		If the content of this address is already of this value, it does not write the byte.
		This function will be longer (read + write) if the byte have to be written,
		and faster (only read) if the content is already correct.
		* @param address EEPROM address to update.
		* @param value object to store at this address, for the size (@see sizeof()) of this object.
		* @return Number of bytes written.
		* @see readAnything()
		* @see updateAnything()
     */
	template <class T> int updateAnything(int address, const T& value)
	{
		eeprom_update_block((const byte*)(const void*)&value, (uint8_t *)INT64 address, sizeof(value));
		return sizeof(value);
	}

	/**
		* Read a string starting from the given address, until its end of the maximum length given.
		* @param address EEPROM address to read.
		* @param outString char array to fill with the bytes loaded from EEPROM memory.
		* @param inMaxLen Maximum size of the string, which should be the size of the recepting buffer set in outString parameter.
		* @return Number of bytes loaded.
		* @see writeString()
		* @see updateString()
     */
	char *readString(int address, char *outString, int inMaxLen);
	/**
		* Write a string starting to the given address until its end.
		* @param address EEPROM address to begin to write to.
		* @param inString char array to store in EEPROM memory.
		* @return Number of bytes written.
		* @see readString()
		* @see updateString()
     */
	int writeString(int address, const char *inString);
	/**
		* Update a string starting to the given address until its end.
		If the content of this address is already of this value, it does not write the byte.
		This function will be longer (read + write) if the byte have to be written,
		and faster (only read) if the content is already correct.
		* @param address EEPROM address to begin to update.
		* @param inString char array to store in EEPROM memory.
		* @return Number of bytes written.
		* @see readString()
		* @see updateString()
     */
	int updateString(int address, const char *inString);

	/**
		* Clear the content of the memory, starting from the given address for the given size.
		Clear means fill the memory with 0s, or with the given byte value.
		* @param address EEPROM address to begin to clear.
		* @param inSize size of memory to clear.
		* @param inFillCharacter chracter to write in this memory area. 0 is the default.
     */
	void clear(int address, int inSize, byte inFillCharacter = 0);
};

extern EEPROMextentClass EEPROMextent;

#include "EEPROM_ItemList.hpp"
#include "CircularBuffer.hpp"

#endif
