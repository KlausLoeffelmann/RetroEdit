#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <cbm.h>

#include "global.h"

// The current line buffer.
// A line has 255 chars max. When it's time to commit the line,
// it's getting transferred into segments of SEGMENT_TEXT_LENGTH
// Bytes.
char _lineBuffer[MAX_LINE_LENGTH];
char _workingLineBuffer[MAX_LINE_LENGTH];

// Pointer to the array holding the point to the start
// and the length of each line.
EditorLine *_editorLines;

// Count of EditorLine pointers.
unsigned int _editorLinesCapacity;

// The pointer to the EditorLineSegments holding
// the actual text in memory.
LineSegment *_editorLineSegments;

// This does not point unconditionally to the next available Segment,
// but point only to a segment, which is no longer in use (so has been freed).
LineSegment *_firstFreeSegment;

unsigned int _maxLineSegment;

// The current TextPosition.
TextPosition _textPos;

// The current screensize.
ScreenSize _screenSize;

// The line number in which the status bar is at.
unsigned char _statusBarLineNo;
unsigned char _debugLineNo;

// The max lines we can use.
unsigned int _maxLine;

PullDownMenu _pullDownMenu;

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
