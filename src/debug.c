#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <cbm.h>

#include "screentools.h"
#include "textgui.h"
#include "debug.h"

TextWindow *_debugTextWindow;

void DefineDebugWindow(unsigned char line, unsigned char col, unsigned char width, unsigned char height)
{
    _debugTextWindow = DefineTextWindow(line, col, width, height);
}

void DebugPrint(char *text)
{
}
