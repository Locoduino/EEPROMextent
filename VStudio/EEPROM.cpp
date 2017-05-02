#include <string.h>
#include <time.h>
#include <stdio.h>

#include "arduino.h"
#include "EEPROMextent.h"

//////////////////////////////////////////
// EEPROM

#define EEPROM_FILE	"d:\\EEPROM.txt"

byte eeprom_image[4096];

EEPROMextentClass::EEPROMextentClass()
{
	FILE *ptr_file;
	fopen_s(&ptr_file, EEPROM_FILE, "r");

	if (!ptr_file)
	{
		memset(eeprom_image, 0, 4096);
		return;
	}

	fread(eeprom_image, 1, 4096, ptr_file);
	fclose(ptr_file);
}

uint8_t eeprom_read_byte(uint8_t *inAddress)
{ 
	return eeprom_image[(int)INT64 inAddress];
}

void writeFile()
{
	FILE *ptr_file;
	fopen_s(&ptr_file, EEPROM_FILE, "w+");

	if (!ptr_file)
	{
		//ASSERT("EEPROM Error ");
		return;
	}

	fwrite(eeprom_image, 1, 4096, ptr_file);
	fclose(ptr_file);
}

void eeprom_write_byte(uint8_t *inAddress, uint8_t inValue)
{
	eeprom_image[(int)INT64 inAddress] = inValue;
	writeFile();
}

void eeprom_update_byte(uint8_t *inAddress, uint8_t inValue)
{
	if (eeprom_read_byte(inAddress) == inValue)
		return;

	eeprom_write_byte(inAddress, inValue);
}

void eeprom_write_block(const uint8_t *inSrc, uint8_t *inDst, size_t inSize)
{
	const byte* p = (const byte*)inSrc;
	int dst = (int)INT64 inDst;
	for (size_t i = 0; i < inSize; i++)
		eeprom_image[dst++] = *(p++);
	writeFile();
}

void eeprom_read_block(uint8_t *inDst, const uint8_t *inSrc, size_t inSize)
{
	byte* p = (byte*)inDst;
	int src = (int)INT64 inSrc;
	for (size_t i = 0; i < inSize; i++)
		*(p++) = eeprom_image[src++];
}

void eeprom_update_block(const uint8_t *inSrc, uint8_t *inDst, size_t inSize)
{
	eeprom_write_block(inSrc, inDst, inSize);
}