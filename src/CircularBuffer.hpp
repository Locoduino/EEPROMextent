//-------------------------------------------------------------------
#ifndef __EEPROM_CircularBuffer_H__
#define __EEPROM_CircularBuffer_H__
//-------------------------------------------------------------------

#include "EEPROMextent.h"

class CircularBuffer
{
public:
	int StartListPos;
	byte ItemSize;
	byte ReplicaNumber;

	CircularBuffer() { this->ItemSize = 0; this->ReplicaNumber = 0; this->StartListPos = 0; }

	int begin(int inStartListPos, byte inItemSize, byte inReplicaNumber) 
	{ 
		this->StartListPos = inStartListPos;  this->ItemSize = inItemSize; this->ReplicaNumber = inReplicaNumber; 
		return (this->ItemSize + 1) * this->ReplicaNumber;
	}

	void *read(void* outpObject);
	void write(void* inpObject, bool inUpdate = false);
	inline void update(void *inpObject) { write(inpObject, true); }

	int getStartRead();
	int startWrite();

	void clear() const;

#ifdef EEPROMEXTENT_DEBUG_MODE
	void printStatus();
#endif

private:
	byte FindEnd();
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
