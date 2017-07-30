#include "EEPROMextent.h"

struct Point
{
	bool exists;
	int number;
	unsigned long delay;
};

CircularBuffer cb;

void setup()
{
	// Writing data...
	Point p1, p2;

	p1.delay = 12345;
	p1.exists = true;
	p1.number = 789;

	// ----------------------------
	// Writing buffer

	int totalsize = cb.begin(10, sizeof(Point), 4);
	Serial.print("Total size of the buffer : ");
	Serial.println(totalsize);

	//cb.Clear();

	cb.update(&p1);
#ifdef EEPROMEXTENT_DEBUG_MODE
	cb.printStatus();
#endif
	cb.read(&p2);

	if (p2.number != 789 || p2.delay != 12345 || p2.exists != true)
		Serial.println("CB ERROR !!");

	p2.delay++;
	p2.exists = false;
	p2.number++;

	cb.update(&p2);
#ifdef EEPROMEXTENT_DEBUG_MODE
	cb.printStatus();
#endif
	cb.read(&p1);

	if (p1.number != 790 || p1.delay != 12346 || p1.exists != false)
		Serial.println("CB ERROR !!");

	// --------------------------
	// Position only buffer

	p1.delay = 12345;
	p1.exists = true;
	p1.number = 789;

	int pos = cb.startWrite();
	EEPROMextent.writeAnything(pos, p1);
	pos = cb.getStartRead();
	cb.read(&p2);

	if (p2.number != 789 || p2.delay != 12345 || p2.exists != true)
		Serial.println("CB ERROR !!");

	p2.delay++;
	p2.exists = false;
	p2.number++;

	for (int i = 0; i < 10; i++)
		pos = cb.startWrite();

	EEPROMextent.writeAnything(pos, p2);
	pos = cb.getStartRead();
	cb.read(&p1);

	if (p1.number != 790 || p1.delay != 12346 || p1.exists != false)
		Serial.println("CB ERROR !!");
}

void loop()
{
}
