//-------------------------------------------------------------------
#ifndef __EEPROM_ItemList_H__
#define __EEPROM_ItemList_H__
//-------------------------------------------------------------------

#define EEPROMLIST_EMPTY_OWNER	255

#include "arduino.h"
#include "EEPROMextent.h"

/**
This class can be used to save heterogeneous items form a hierarchical list..

The base of the data is a list of items of the same size in EEPROM. This size have to be the greatest size among all
items to save. The list is initialized with the total size of memory dedicated to this list.
The number of items which must be lower than 255, so the total size must be lower than (item size + 2) * 255 !
Each item is identified by a number, the slot number betw<een 0 and the maximum - 1 .

\verbatim
    0      1      2      ...     max
+------+------+------+---------------+
|TPitem|TPitem|TPitem|........|TPitem|
+------+------+------+--------+------+
\endverbatim

Each item starts with a type on a byte (the 'T' in the schema). A type at 0 means empty slot.
The following byte 'P' gives an optionnal parent slot number. A parent at 255 means no parent.
And finaly, the item itself fill the rest of the 'slot' place.
Parent and children can be separated and even saved in a random order...
*/
class EEPROM_ItemListClass
{
protected:
	int startListPos;
	byte itemSize;
	int listSize;

public:
	/** Default constructor. */
	EEPROM_ItemListClass() { this->itemSize = 0; this->listSize = 0; this->startListPos = 0; }

	/** Initialize the instance.
	@param inStartListPos	EEPROM address of the begginning of the memory used for the storage
	@param inItemSize Size in bytes of one item to save.
	@param inListSize Number of bytes of EEPROM used for the total list.
	*/
	void begin(int inStartListPos, byte inItemSize, int inListSize) { this->startListPos = inStartListPos;  this->itemSize = inItemSize; this->listSize = inListSize; }

	/** Reset the given slot and children to empty.
	@param inSlotNumber	This slot will be declared as empty. If this item have children, there will be freed too.
	*/
	void FreeItem(byte inSlotNumber);
	/** Reset all the children of the given slot to empty. The slot itself will not be freed.
	@param inOwnerSlotNumber	Owner of the slots to free.
	*/
	void FreeOwnedItems(byte inOwnerSlotNumber);

	/** Gets the absolute EEPROM address for the given slot's item.
	@param inSlotNumber	Slot number to check.
	@return EEPROM address of the given slot item, AFTER the type and parent bytes.
	*/
	inline int GetItemPos(byte inSlotNumber)
	{
		return GetItemPosRaw(inSlotNumber) + 2;
	}

	/** Gets the absolute EEPROM address for the full given slot.
	@param inSlotNumber	Slot number to check.
	@return EEPROM address of the type byte of the given slot.
	*/
	inline int GetItemPosRaw(byte inSlotNumber)
	{
		return (inSlotNumber * itemSize) + startListPos;
	}

	/** Gets the slot number from an absolute EEPROM address.
	@param inPosition EEPROM address.
	@return Slot number.
	*/
	inline byte GetSlotFromPos(int inPosition)
	{
		return (inPosition - startListPos) / itemSize;
	}

	/** Gets the type of the slot item.
	@param inSlotNumber slot to check.
	@return Type (byte 'T') of the slot item.
	*/
	inline byte GetItemType(byte inSlotNumber)
	{
		return EEPROMextent.readByte(GetItemPosRaw(inSlotNumber));
	}

	/** Gets the parent of the slot item.
	@param inSlotNumber slot to check.
	@return Parent id (byte 'P') of the slot item, 255 if no parent.
	*/
	inline byte GetItemOwner(byte inSlotNumber)
	{
		return EEPROMextent.readByte(GetItemPosRaw(inSlotNumber) + 1);
	}

	/** Save the prefix (bytes T and P) of this item.
	@param inSlotNumber slot to write.
	@param inType type byte to write.
	@param inOwner parent slot number to write, 255 if no parent.
	@return EEPROM address to start writing the item.
	*/
	int SaveItemPrefix(byte inSlotNumber, byte inType, byte inOwner);

	/** Gets the first free slot number.
	@return First free slot address or 255 if nothing free.
	*/
	byte GetFirstFreeSlot();

	/** Try to find an item according to its type.
	@param inType type of the item to find.
	@param inStartSlotNumber Starting position for the research. Default 0.
	@param inOwnerId Try to find a child of the given owner id. Default is 255, meaning no research by owner id.
	@return First slot number corresponding to the criterias or 255 if nothing found.
	*/
	byte FindItem(byte inType, byte inStartSlotNumber = 0, byte inOwnerId = 255);

	/** Counts the used slot number.
	@param inType Counts all items of this type. 0 means all types.
	@return Number of items of the given type, or total number of items if inType is 0.
	*/
	byte CountItems(byte inType);

	/** Counts the children of the given item id.
	@param inOwnerId Parent id.
	@return Number of items with inOwnerId as parent.
	*/
	byte CountOwnedItems(byte inOwnerId);

	/** Clears all the EEPROM memory area reserved for this list.
	*/
	void clear() const;
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
