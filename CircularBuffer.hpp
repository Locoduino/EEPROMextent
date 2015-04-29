//-------------------------------------------------------------------
#ifndef __EEPROM_CircularBuffer_H__
#define __EEPROM_CircularBuffer_H__
//-------------------------------------------------------------------

//-------------------------------------------------------------------

//-------------------------------------------------------------------

#include "arduino.h"
#include "EEPROMextent.h"

class CircularBuffer
{
public:
	int StartListPos;
	byte ItemSize;
	byte ReplicaNumber;

	CircularBuffer() { this->ItemSize = 0; this->ReplicaNumber = 0; this->StartListPos = 0; }

	int Setup(int inStartListPos, byte inItemSize, byte inReplicaNumber) 
	{ 
		this->StartListPos = inStartListPos;  this->ItemSize = inItemSize; this->ReplicaNumber = inReplicaNumber; 
		return (this->ItemSize + 1) * this->ReplicaNumber;
	}

	void *Read(void* outpObject);
	void Write(void* inpObject);
	void Update(void *inpObject);

	void Clear() const;

	void printStatus();

private:
	byte FindEnd();
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
