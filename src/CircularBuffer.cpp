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

	int prevpos = this->StartListPos + this->ReplicaNumber - 1;
	byte prev = EEPROMextent.read(prevpos);

	for (int i = 0; i < this->ReplicaNumber; i++)
	{
		int pos = this->StartListPos + i;

		// Checks it the current value is really the previous value + 1 : 
		// 4 must be 3+1, 0 must be 255 + 1 !
		if (prev + 1 != EEPROMextent.read(pos))
			return prevpos - this->StartListPos;

		prev = EEPROMextent.read(pos);
		prevpos = pos;
	}

	// Should never reach this code !

	return 255;
}

void * CircularBuffer::Read(void* outpData)
{
	byte place = FindEnd();
	eeprom_read_block((uint8_t *)outpData, (const uint8_t *)(this->StartListPos + this->ReplicaNumber + (this->ItemSize * place)), this->ItemSize);

	return outpData;
}

void CircularBuffer::Write(void* inpData, bool inUpdate)
{
	byte place = FindEnd();
	byte itemNb = EEPROMextent.read(this->StartListPos + place);

	place++;
	if (place >= this->ReplicaNumber)
		place = 0;

	if (inUpdate)
	{
		EEPROMextent.update(this->StartListPos + place, ++itemNb);
		eeprom_update_block((const uint8_t *)inpData, (uint8_t *)(this->StartListPos + this->ReplicaNumber + (this->ItemSize * place)), this->ItemSize);
	}
	else
	{
		EEPROMextent.write(this->StartListPos + place, ++itemNb);
		eeprom_write_block((const uint8_t *)inpData, (uint8_t *)(this->StartListPos + this->ReplicaNumber + (this->ItemSize * place)), this->ItemSize);
	}
}

void CircularBuffer::Clear() const
{
	EEPROMextent.clear(this->StartListPos, (this->ItemSize + 1) * this->ReplicaNumber);
}

#ifdef VISUALSTUDIO
#include "Serial.hpp"
#endif

#ifdef DEBUG_MODE
void CircularBuffer::printStatus()
{
	Serial.print(F("CB Status : "));
	for (int i = 0; i < this->ReplicaNumber; i++)
	{
		Serial.print(F("|"));
		Serial.print(EEPROMextent.read(this->StartListPos + i));
	}
	Serial.println(F("|"));
}
#endif



