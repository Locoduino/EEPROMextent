#include <string.h>
#include <time.h>

#include "arduino.h"
#include "Serial.hpp"

//////////////////////////////////////////
// Arduino

void pinMode(int, int)
{}

void digitalWrite(int, int)
{}

int digitalRead(int)
{
	return 0;
}

void analogWrite(int, int)
{}

int analogRead(int)
{
	return 0;
}

unsigned long millis()
{
	time_t t;
	time(&t);
	return (unsigned long)t * 1000;
}

unsigned long micros()
{
	time_t t;
	time(&t);
	return (unsigned long)t * 1000000;
}

void delay(int)
{}

void interrupts()
{}

void noInterrupts()
{}

void attachInterrupt(uint8_t, void(*)(void), int mode)
{}

void detachInterrupt(uint8_t)
{}

double power(double val, int exp)
{
	double res = val;
	for (int i = 0; i < exp; i++)
		res *= val;

	return res;
}

//////////////////////////////////////////
// Memory

int freeMemory()
{
	return 20000;
}


int map(int, int, int, int, int)
{
	return 0;
}

byte pgm_read_byte(const char *str)
{
	return (byte)*str;
}

int pgm_read_word(const char *str)
{
	return (int)str;
}

//////////////////////////////////////////
// Serial

SerialClass SerialClass::SerialInstance;

String memo;

void SerialClass::print(const char *line)
{
	memo += line;
}

void SerialClass::print(const String &line)
{
	memo += line;
}

void SerialClass::print(int value, int i)
{
	int base = 10;
	switch (i)
	{
	case DEC:
		base = 10;
		break;
	case HEX:
		base = 16;
		break;
	case BIN:
		base = 2;
		break;
	}
	print(String(value, base));
}

void SerialClass::println(int value, int i)
{
	print(value, i);
	println("");
}

#undef INPUT
#undef OUTPUT
#undef HEX
#undef byte
#undef boolean

#include "EEPROMex_VStudioMfcDlg.h"

void SerialClass::println(const char *line)
{
	memo += line;
	char buf[200];
	memo.toCharArray(buf, 200);
	EEPROMex_VStudioMfcDlg::pMainDialog->debug(buf);
	memo = "";
}

void SerialClass::println(const String &line)
{
	memo += line;
	char buf[200];
	memo.toCharArray(buf, 200);
	EEPROMex_VStudioMfcDlg::pMainDialog->debug(buf);
	memo = "";
}

