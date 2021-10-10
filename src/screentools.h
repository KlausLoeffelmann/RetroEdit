#include "global.h"

void ClearScreen();
void ClearScreenEx(unsigned char column, unsigned char line, unsigned char width, unsigned char height, char clearChar);
void DrawWindow(unsigned char column, unsigned char line, unsigned char width, unsigned char height, unsigned char color);
unsigned char PetAsciiToScreenCode(unsigned char petAscii);
