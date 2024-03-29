#define CHAR_HORIZONTAL_LINE 64
#define CHAR_VERTICAL_LINE 93

void ClearScreen();
void ClearScreenEx(unsigned char column, unsigned char line, unsigned char width, unsigned char height, char clearChar);
void DrawHLine(unsigned char column, unsigned char line, unsigned char length, unsigned char lineChar);
unsigned char DrawUIText(char *text, unsigned char column, unsigned char line, unsigned char color);
void DrawWindow(unsigned char column, unsigned char line, unsigned char width, unsigned char height, unsigned char color);
unsigned char GetUITextLength(char *text);
unsigned char PetAsciiToScreenCode(unsigned char petAscii);
