/*************************************************************
project: <EEPROMextent>
author: <Thierry PARIS>
description: <Class for a circular buffer of data stored in EEPROM>
*************************************************************/

#include "CircularBuffer.hpp"

/*
A circular buffer is a way to reduce the use of each EEPROM cell,
and improve life time of the full EEPROM memory.
An index of bytes is stored at the beginning of the area.
Each index represents a data area.

|0|1|2|3|4||---0---|---1---|---2---|---3---|---4---|
*/

byte CircularBuffer::FindEnd()
{
	/* 
	prev is the previous value from the item in the list
	|..|..|prev|i|..|..|

	we begin with the first item of the list. In this case, the previous is the last one:
	|i|..|..|..|..|prev|
	*/

	int prevpos = this->startListPos + this->replicaNumber - 1;
	byte prev = EEPROMextent.readByte(prevpos);

	for (int i = 0; i < this->replicaNumber; i++)
	{
		int pos = this->startListPos + i;

		// Checks it the current value is really the previous value + 1 : 
		// 4 must be 3+1, 0 must be 255 + 1 !
		if (prev + 1 != EEPROMextent.readByte(pos))
			return prevpos - this->startListPos;

		prev = EEPROMextent.readByte(pos);
		prevpos = pos;
	}

	// Should never reach this code !

	return 255;
}

void * CircularBuffer::read(void* outpData)
{
	byte place = this->FindEnd();
	eeprom_read_block((uint8_t *)outpData, (const uint8_t *)INT64 (this->startListPos + this->replicaNumber + (this->elementSize * place)), this->elementSize);

	return outpData;
}

void CircularBuffer::write(void* inpData, bool inUpdate)
{
	byte place = this->FindEnd();
	byte itemNb = EEPROMextent.readByte(this->startListPos + place);

	place++;
	if (place >= this->replicaNumber)
		place = 0;

	if (inUpdate)
	{
		EEPROMextent.updateByte(this->startListPos + place, ++itemNb);
		eeprom_update_block((const uint8_t *)inpData, (uint8_t *)INT64 (this->startListPos + this->replicaNumber + (this->elementSize * place)), this->elementSize);
	}
	else
	{
		EEPROMextent.writeByte(this->startListPos + place, ++itemNb);
		eeprom_write_block((const uint8_t *)inpData, (uint8_t *)INT64 (this->startListPos + this->replicaNumber + (this->elementSize * place)), this->elementSize);
	}
}

void CircularBuffer::clear() const
{
	EEPROMextent.clear(this->startListPos, (this->elementSize + 1) * this->replicaNumber);
}

int CircularBuffer::getStartRead()
{
	byte place = this->FindEnd();
	return this->startListPos + this->replicaNumber + (this->elementSize * place);
}

int CircularBuffer::startWrite()
{
	byte place = this->FindEnd();
	byte itemNb = EEPROMextent.readByte(this->startListPos + place);

	place++;
	if (place >= this->replicaNumber)
		place = 0;

	EEPROMextent.updateByte(this->startListPos + place, ++itemNb);

	return this->startListPos + this->replicaNumber + (this->elementSize * place);
}

#ifdef VISUALSTUDIO
//#include "Serial.hpp"
#endif

#ifdef EEPROMEXTENT_DEBUG_MODE
void CircularBuffer::printStatus()
{
	Serial.print(F("CB Status : "));
	for (int i = 0; i < this->replicaNumber; i++)
	{
		Serial.print(F("|"));
		Serial.print(EEPROMextent.readByte(this->startListPos + i));
	}
	Serial.println(F("|"));
}
#endif



