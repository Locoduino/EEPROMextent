#include "EEPROMextent.h"

EEPROM_ItemListClass ItemList;

struct parent
{
	byte id;
	int number;
	char text[10];
};

struct child
{
	byte id;
	char descr[10];
	int dummy;
};

byte save_parent(parent &aParent);
byte save_child(child &aChild, byte aParent);

#define ITEM_SIZE		20

byte save_parent(parent &aParent)
{
	byte p1_place = ItemList.GetFirstFreeSlot();
	// Save the standard prefix at first free place (p1_place).
	// 'P' because this item is a parent.
	// 255 means this item have no parent itself. So 254 is the maximum number of items which can be saved by this library!
	int pos = ItemList.SaveItemPrefix(p1_place, 'P', 255);
	// Then write the parent content (do not forget to update the position 'pos' for each field...):
	EEPROMextent.writeByte(pos++, aParent.id);
	pos += EEPROMextent.writeAnything(pos, aParent.number);
	EEPROMextent.writeString(pos, aParent.text);
	pos += 10;

	if (pos - ItemList.GetItemPosRaw(p1_place) > ITEM_SIZE)
	{
		Serial.print("Too long parent record : increase the item size to ");
		Serial.print(pos - ItemList.GetItemPosRaw(p1_place));
		Serial.println(" minimum !");
	}

	return p1_place;
}

byte save_child(child &aChild, byte aParent)
{
	byte c1_place = ItemList.GetFirstFreeSlot();
	// Save the standard prefix at first free place (c1_place).
	// 'C' because this item is a child.
	// p1_place means that the parent of this item is the one at p1_place.
	int pos = ItemList.SaveItemPrefix(c1_place, 'C', aParent);
	// Then write the child content (do not forget to update the position 'pos' for each field...):
	EEPROMextent.writeByte(pos++, aChild.id);
	EEPROMextent.writeString(pos, aChild.descr);
	pos += 10; // maximum size of descr string as in its declaration.
	pos += EEPROMextent.writeAnything(pos, aChild.dummy);

	if (pos - ItemList.GetItemPosRaw(c1_place) > ITEM_SIZE)
	{
		Serial.print("Too long child record : increase the item size to ");
		Serial.print(pos - ItemList.GetItemPosRaw(c1_place));
		Serial.println(" minimum !");
	}

	return c1_place;
}

void setup()
{
	// Estimated size of 15 bytes for each parent and each child...
	// Arbitrary start position for writing in EEPROM.
	// Arbitrary EEPROM size, must be adapted according to your Arduino model.
	ItemList.begin(10, ITEM_SIZE, 1024);

	parent p1;
	p1.id = 0;
	p1.number = 123;
	strcpy(p1.text, "Bonjour");

	child c1;
	c1.id = 100;
	c1.dummy = 123;
	strcpy(c1.descr, "Child1");

	child c2;
	c2.id = 101;
	c2.dummy = 456;
	strcpy(c2.descr, "Child2");

	parent p2;
	p2.id = 1;
	p2.number = 78;
	strcpy(p2.text, "la France");

	child c3;
	c3.id = 102;
	c3.dummy = 89;
	strcpy(c3.descr, "Child3");

	child c4;
	c4.id = 103;
	c4.dummy = 1000;
	strcpy(c4.descr, "Child4");

	child c5;
	c5.id = 104;
	c5.dummy = 1001;
	strcpy(c5.descr, "Child5");

	// Clear the data area.
	ItemList.clear();

	// Writing data...
	// save the parent p1
	byte p1_place = save_parent(p1);

	// save the first child c1
	save_child(c1, p1_place);

	// save the second child c2
	save_child(c2, p1_place);

	// save the parent p2
	byte p2_place = save_parent(p2);

	// save the first child c2
	save_child(c3, p2_place);

	// save the second child c3
	save_child(c4, p2_place);

	// save the third child c5
	save_child(c5, p2_place);

	// Map of the items in EEPROM:
	// 
	// slots	0  1  2	 3	4  5  6   7(first free)
	// items	P1 C1 C2 P2 C3 C4 C5

	Serial.print("First free should be 7 : ");
	Serial.println(ItemList.GetFirstFreeSlot());

	Serial.print("Number of parents should be 2 : ");
	Serial.println(ItemList.CountItems('P'));

	Serial.print("Number of children should be 5 : ");
	Serial.println(ItemList.CountItems('C'));

	Serial.print("Number of children of p1 should be 2 : ");
	Serial.println(ItemList.CountOwnedItems(p1_place));

	Serial.print("Number of children of p2 should be 3 : ");
	Serial.println(ItemList.CountOwnedItems(p2_place));
}

void loop()
{
}