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
	gotoxy(1, _debugLineNo);
	printf(text, value);
	for (value = 0; value < 20000; value++)
	{
	}
}
