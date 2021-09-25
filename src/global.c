#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <cbm.h>

#include "global.h"

void DebugPrint(const char *text, int value)
{
	gotoxy(1, _debugLineNo);
	printf(text, value);
}

void DebugPrintSlow(const char *text, int value)
{
	unsigned char loopMax;

#if __C64__
	loopMax = 5000;
#else
	loopMax = 20000;
#endif

	gotoxy(1, _debugLineNo);
	printf(text, value);

	for (value = 0; value < loopMax; value++)
	{
	}
}
