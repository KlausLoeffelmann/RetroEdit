#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <cbm.h>

#include "screentools.h"

#if __C64__
#define SCREENMEM 0x0A00
#else
#define SCREENMEM 0x0A00
#endif

void ClearScreen(unsigned char column, unsigned char line, unsigned char width, unsigned char height)
{
	unsigned char *screenMem;
	unsigned char x, y;

	screenMem = (unsigned char *)SCREENMEM + _screenSize.Width * line + column;
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			*screenMem++ = 32;
		}
		screenMem += (_screenSize.Width - column);
	}
}

void DrawWindow(unsigned char column, unsigned char line, unsigned char width, unsigned char height, char *titel)
{
}
