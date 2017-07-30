/*************************************************************
project: <EEPROMextent>
author: <Thierry PARIS>
description: <Class for basic EEPROM functions>
*************************************************************/

#include "arduino.h"
#include "EEPROMextent.h"

EEPROMextentClass EEPROMextent;

char *EEPROMextentClass::readString(int ee, char *outData, int inSize)
{
	byte* p = (byte*)outData;
	for (int i = 0; i < inSize; i++)
	{
		*p = eeprom_read_byte((uint8_t *) INT64 ee++);

		if (*p == 0)
			break;
		p++;
	}

	return outData;
}

int EEPROMextentClass::writeString(int ee, const char *inData)
{
	const byte* p = (const byte*)inData;
	int i;

	for (i = 0;; i++)
	{
		const byte b = *p;
		eeprom_write_byte((uint8_t *) INT64 ee++, b), ++p;

		if (b == 0)
			return i + 1;
	}

	return i;
}

int EEPROMextentClass::updateString(int ee, const char *inData)
{
	const byte* p = (const byte*)inData;
	int i;

	for (i = 0;; i++)
	{
		const byte b = *p;
		eeprom_update_byte((uint8_t *) INT64 ee++, b), ++p;

		if (b == 0)
			return i + 1;
	}

	return i;
}

void EEPROMextentClass::clear(int inStartingAddress, int inSize, byte inFillCharacter)
{
	for (int i = inStartingAddress; i < inStartingAddress + inSize; i++)
		eeprom_update_byte((uint8_t *)INT64 i, inFillCharacter);
}
