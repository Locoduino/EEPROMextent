/*************************************************************
project: <EEPROMextent>
author: <Thierry PARIS>
description: <Class for a full hierarchical list of items of the same size stored in EEPROM>
*************************************************************/

#include "EEPROM_ItemList.hpp"

/*
The item list area of the EEPROM is a long list of items of the same size.
Each item can be identified by a type.
Each item has its own structure, but is stored in a 'slot' of EEPROM_ItemList::ItemSize.
Each item slot begins with two bytes : the type, and the owner (255 if null)
This owner gives the ability to build a hierarchical item tree !
The owner id is in reality the address of the slot of the owner !
*/

void EEPROM_ItemListClass::FreeItem(byte inSlotNumber)
{
	// Replace the type by '0' : free slot !
	EEPROMextent.updateByte(GetItemPosRaw(inSlotNumber), 0);
	FreeOwnedItems(inSlotNumber);
}

void EEPROM_ItemListClass::FreeOwnedItems(byte inOwnerSlotNumber)
{
	int slot = 0;
	for (int pos = this->startListPos; pos < this->startListPos + this->listSize; pos += itemSize)
	{
		if (EEPROMextent.readByte(pos) == 0)
			continue;
		if (EEPROMextent.readByte(pos + 1) == inOwnerSlotNumber)
		{
			// Replace the type by '0' : free slot !
			EEPROMextent.updateByte(pos, 0);
			// also remove the owned items, and their owned items in turn...
			FreeItem(slot);
		}
		slot++;
	}
}

byte EEPROM_ItemListClass::FindItem(byte inType, byte inStartSlotNumber, byte inOwnerId)
{
	byte slot = inStartSlotNumber;

	for (int pos = startListPos + (inStartSlotNumber * itemSize); pos < this->startListPos + this->listSize; pos += itemSize)
	{
		if (EEPROMextent.readByte(pos) == inType)
			if (inOwnerId == 255 || EEPROMextent.readByte(pos + 1) == inOwnerId)
				return slot;
		slot++;
	}

	return 255;
}

byte EEPROM_ItemListClass::CountItems(byte inType)
{
	byte nb = 0;

	for (int pos = startListPos; pos < this->startListPos + this->listSize; pos += itemSize)
		if (inType == 0 || EEPROMextent.readByte(pos) == inType)
			nb++;

	return nb;
}

byte EEPROM_ItemListClass::CountOwnedItems(byte inOwnerId)
{
	byte nb = 0;

	for (int pos = startListPos; pos < this->startListPos + this->listSize; pos += itemSize)
		if (EEPROMextent.readByte(pos) != 0 && EEPROMextent.readByte(pos + 1) == inOwnerId)
			nb++;

	return nb;
}

byte EEPROM_ItemListClass::GetFirstFreeSlot()
{
	byte slot = 0;

	for (int pos = startListPos; pos < this->startListPos + this->listSize; pos += itemSize)
	{
		byte val = EEPROMextent.readByte(pos);
		if (val == 0)
			return slot;
		slot++;
	}

	return 255;
}

int EEPROM_ItemListClass::SaveItemPrefix(byte inSlotNumber, byte inType, byte inOwner)
{
	int pos = GetItemPosRaw(inSlotNumber);
	EEPROMextent.updateByte(pos++, inType);
	EEPROMextent.updateByte(pos++, inOwner);
	return pos;
}

void EEPROM_ItemListClass::clear() const
{
	EEPROMextent.clear(this->startListPos, this->listSize);
}



