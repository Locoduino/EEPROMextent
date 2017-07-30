#include "EEPROMextent.h"

struct Point
{
	bool exists;
	int number;
	unsigned long delay;
};

void setup()
{
    Serial.begin(115200);
        
	// Clear the data area.
	EEPROMextent.clear(0, 100);

	// Writing data...
	Point p1;

	p1.delay = 12345;
	p1.exists = true;
	p1.number = 789;

	EEPROMextent.writeByte(0, 10);
	EEPROMextent.writeByte(1, 11);

	EEPROMextent.writeAnything(10, p1);
	EEPROMextent.writeString(50, "Bonjour la France");

	// Reading the writen data.
	Point p2;
	char text[100];

	byte read0 = EEPROMextent.readByte(0);
	byte read1 = EEPROMextent.readByte(1);

	EEPROMextent.readAnything(10, p2);
	EEPROMextent.readString(50, text, 100);

	Serial.print("Should be 10 : ");
	Serial.println(read0);

	Serial.print("Should be 11 : ");
	Serial.println(read1);

	Serial.print("Should be 12345 : ");
	Serial.println(p2.delay);

	Serial.print("Should be 1 : ");
	Serial.println(p2.exists);

	Serial.print("Should be 789 : ");
	Serial.println(p2.number);

	Serial.print("Should be \"Bonjour la Fance\" : ");
	Serial.println(text);
}

void loop()
{
}
