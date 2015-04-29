//-------------------------------------------------------------------
#ifndef __liquidCrystal_H__
#define __liquidCrystal_H__
//-------------------------------------------------------------------

#include <iostream>

class LiquidCrystal
{
public:
	void begin(int chars, int lines) { sizex = chars; sizey = lines; }
	void clear()
	{
		memset(buffer0, 32, sizex);
		memset(buffer1, 32, sizex);
		memset(buffer2, 32, sizex);
		memset(buffer3, 32, sizex);
		Show();
	}

	void print(const char *line)
	{
		char * buffer = (y == 0 ? buffer0 : y == 1 ? buffer1 : y == 2 ? buffer2 : y == 3 ? buffer3 : "");

		int c = 0;
		while (line[c] != 0)
			buffer[x + c] = line[c++];
		buffer[sizex] = 0;
	}

	void print(int value, int i = 0) {}
	void setCursor(int posx, int posy) { x = posx; y = posy; }

	void Show() {
		std::cout << "  0123456789012345" << std::endl;
		for (int i = 0; i < sizey; i++)
			std::cout << i << " " << (i == 0 ? buffer0 : i == 1 ? buffer1 : i == 2 ? buffer2 : i == 3 ? buffer3 :"") << std::endl;
		std::cout << "  0123456789012345" << std::endl << std::endl;
	}

	LiquidCrystal(byte, byte, byte, byte, byte, byte) {}

private:
	char buffer0[40];
	char buffer1[40];
	char buffer2[40];
	char buffer3[40];
	int sizex, sizey;
	int x, y;
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------