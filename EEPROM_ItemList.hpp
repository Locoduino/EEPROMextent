//-------------------------------------------------------------------
#ifndef __EEPROM_ItemList_H__
#define __EEPROM_ItemList_H__
//-------------------------------------------------------------------

//-------------------------------------------------------------------

//-------------------------------------------------------------------

#define EEPROMLIST_EMPTY_OWNER	255

#include "arduino.h"
#include "EEPROMextent.h"

class EEPROM_ItemListClass
{
public:
	int StartListPos;
	byte ItemSize;
	int ListSize;

	EEPROM_ItemListClass() { this->ItemSize = 0; this->ListSize = 0; this->StartListPos = 0; }

	void Setup(int inStartListPos, byte inItemSize, int inListSize) { this->StartListPos = inStartListPos;  this->ItemSize = inItemSize; this->ListSize = inListSize; }

	void FreeItem(byte inSlotNumber);
	void FreeOwnedItems(byte inOwnerSlotNumber);

	inline int GetItemPos(byte inSlotNumber)
	{
		return GetItemPosRaw(inSlotNumber) + 2;
	}

	inline int GetItemPosRaw(byte inSlotNumber)
	{
		return (inSlotNumber * ItemSize) + StartListPos;
	}

	inline byte GetSlotFromPos(int inPosition)
	{
		return (inPosition - StartListPos) / ItemSize;
	}

	inline byte GetItemType(byte inSlotNumber)
	{
		return EEPROMextent.read(GetItemPosRaw(inSlotNumber));
	}

	inline byte GetItemOwner(byte inSlotNumber)
	{
		return EEPROMextent.read(GetItemPosRaw(inSlotNumber) + 1);
	}

	inline int SaveItemPrefix(byte inSlotNumber, byte inType, byte inOwner)
	{
		int pos = GetItemPosRaw(inSlotNumber);
		EEPROMextent.update(pos++, inType);
		EEPROMextent.update(pos++, inOwner);
		return pos;
	}

	byte GetFirstFreeSlot();
	byte FindItem(byte inType, byte inStartSlotNumber = 0, byte inOwnerId = 255);
	byte CountItems(byte inType);
	byte CountOwnedItems(byte inOwnerId);
	char *GetItems();
	void clear() const;
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
