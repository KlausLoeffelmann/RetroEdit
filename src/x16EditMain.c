#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <cbm.h>

#define KEY_RETURN 13
#define KEY_CURUP 145
#define KEY_CURDOWN 17
#define KEY_CURLEFT 157
#define KEY_CURRIGHT 29
#define KEY_ESC 3
#define KEY_BACKSPACE 20
#define KEY_TAB 9

#define SEGMENT_TEXT_LENGTH 10
#define START_EDITOR_LINES 4

#define MAX_LINE_LENGTH 255
#define LINE_NUMBER_OFFSET 6

#define EDITOR_MEMORY 0x9000

// A Segment of SEGMENT_TEXT_LENGTH of bytes for storing text.
typedef struct LineSegment LineSegment;

// The current TextPosition.
typedef struct TextPosition TextPosition;

// The Size of the Screen.
typedef struct Size Size;

// Pointer for each Line: length of Line and pointer to first line segment.
typedef struct EditorLine EditorLine;

struct EditorLine
{
	unsigned char length;
	LineSegment *firstLineSegment;
};

struct LineSegment
{
	char text[SEGMENT_TEXT_LENGTH];
	LineSegment *nextSegment;
};

struct Size
{
	unsigned char Width;
	unsigned char Height;
};

struct TextPosition
{
	unsigned int Line;
	unsigned char Column;
	unsigned char ScreenLine;
	unsigned char ScreenColumn;
	unsigned char LineLength;
};

// The current line buffer.
// A line has 255 chars max. When it's time to commit the line,
// it's getting transferred into segments of SEGMENT_TEXT_LENGTH
// Bytes.
char _lineBuffer[255];

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
Size _screenSize;

// The line number in which the status bar is at.
unsigned char _statusBarLineNo;
unsigned char _debugLineNo;

// The max lines we can use.
unsigned int _maxLine;

void DebugPrint(const char *text, int value)
{
	gotoxy(1, _debugLineNo);
	printf(text, value);
}

void UpdateDocInfo(int line, int column, char currentChar)
{
	LineSegment *tmpLines = _editorLineSegments;

	gotoxy(1, _statusBarLineNo);
	printf("Line: %04d", line);
	gotoxy(15, _statusBarLineNo);
	printf("Column: %03d", column);
	gotoxy(30, _statusBarLineNo);
	printf("Keycode: %03d", currentChar);

	gotoxy(43, _statusBarLineNo);
	tmpLines += _maxLineSegment;
	printf("Lines-*: %X/%X", tmpLines, _firstFreeSegment);

	gotoxy(62, _statusBarLineNo);
	printf("maxLS: %d", _maxLineSegment);

	gotoxy(
		_textPos.ScreenColumn + LINE_NUMBER_OFFSET,
		_textPos.ScreenLine);
}

void EnsureEditorLinesCapacity(char init)
{
	// Dynamic allocate memory for the Lines pointer:
	if (_maxLine > _editorLinesCapacity || init == 1)
	{
		_editorLinesCapacity = _editorLinesCapacity + _editorLinesCapacity;
		_editorLines = realloc(_editorLines, sizeof(EditorLine) * _editorLinesCapacity);
	}
}

void Initialize()
{
	_maxLine = 0;
	_textPos.Column = 0;
	_textPos.Line = 0;
	_textPos.ScreenColumn = 0;
	_textPos.ScreenLine = 0;

	_editorLinesCapacity = START_EDITOR_LINES;
	EnsureEditorLinesCapacity(1); // init:1

	screensize(&_screenSize.Width, &_screenSize.Height);
	_screenSize.Width = _screenSize.Width - LINE_NUMBER_OFFSET - 1;
	_statusBarLineNo = _screenSize.Height - 1;
	_debugLineNo = _screenSize.Height - 2;

	clrscr();
	cursor(1);

	_maxLineSegment = 0;
	_editorLineSegments = (LineSegment *)EDITOR_MEMORY;
	_firstFreeSegment = NULL;
}

void PrintLineNumber()
{
	gotoxy(0, _textPos.ScreenLine);
	printf("%04d:", _textPos.Line + 1);
}

LineSegment *GetNextFreeLineSegment()
{
	LineSegment *nextFreeSegment;

	if (_firstFreeSegment == NULL)
	{
		// What does this do?
		nextFreeSegment = _editorLineSegments + _maxLineSegment;
		_maxLineSegment++;
	}
	else
	{
		nextFreeSegment = _firstFreeSegment;
		_firstFreeSegment = _firstFreeSegment->nextSegment;
		DebugPrint("FFS: %X", (int)_firstFreeSegment);
	}

	return nextFreeSegment;
}

void SaveBufferToEditorMemory()
{
	unsigned char y;
	unsigned char i;

	LineSegment *nextLineSegment;

	// We need to expand the collection with LineStart pointers.
	EnsureEditorLinesCapacity(0);
	nextLineSegment = GetNextFreeLineSegment();
	DebugPrint("currentLineSegment Address: %X", (int)nextLineSegment);

	_editorLines[_textPos.Line].firstLineSegment = nextLineSegment;

	y = 0;
	for (i = 0; i < _textPos.Column; i++)
	{
		if (y == SEGMENT_TEXT_LENGTH)
		{
			nextLineSegment = GetNextFreeLineSegment();
			y = 0;
		}
		nextLineSegment->text[y++] = _lineBuffer[i];
	}
	_editorLines[_textPos.Line].length = _textPos.Column;
}

void GetBufferFromEditorMemoryAndPrint()
{
	LineSegment *currentLineSegment, *tmpLineSegment;
	unsigned char lineLength;
	unsigned char charpoint;
	unsigned char i;
	unsigned char columnOffset;

	// Free the line, if it is not a new line we're saving.
	// This is either NULL or what ever segment we have already allocated.
	// It is supposed to be NULL when we appended a new line at the end.
	if (_firstFreeSegment != NULL)
	{
		// We need to traverse everything until its hit NULL
		// and then link that segment with the segments occupied
		// by this line:
		tmpLineSegment = _firstFreeSegment;
		while (tmpLineSegment->nextSegment != NULL)
		{
			tmpLineSegment = tmpLineSegment->nextSegment;
		}

		// This is, because there might have been free segments left from
		// the last edit which resulted in less segments then there were before.
		tmpLineSegment->nextSegment = _editorLines[_textPos.Line].firstLineSegment;
	}

	// If we adding new lines, this will stay NULL. If we move around and edit,
	// this leads to reusing the existing segments.
	_firstFreeSegment = _editorLines[_textPos.Line].firstLineSegment;

	columnOffset = _textPos.Column - _textPos.ScreenColumn;
	currentLineSegment = _editorLines[_textPos.Line].firstLineSegment;
	lineLength = _editorLines[_textPos.Line].length;
	_textPos.LineLength = lineLength;
	charpoint = 0;

	gotoxy(LINE_NUMBER_OFFSET, _textPos.ScreenLine);

	for (i = 0; i < MAX_LINE_LENGTH; i++)
	{
		if (i < lineLength)
		{
			_lineBuffer[i] = currentLineSegment->text[charpoint++];
			if (charpoint == SEGMENT_TEXT_LENGTH)
			{
				currentLineSegment = currentLineSegment->nextSegment;
				charpoint = 0;
			}
		}
		else
		{
			_lineBuffer[i] = 32;
		}

		if (i >= columnOffset && i <= (columnOffset + _screenSize.Width))
		{
			cputc(_lineBuffer[i]);
		}
	}
}

// Repaints the whole screen
void Invalidate()
{
}

void LineBufferToCurrentScreenLine()
{
	unsigned char i;
	unsigned char leftTextWindowPos, rightTextWindowPos;

	leftTextWindowPos = _textPos.Column - _textPos.ScreenColumn;
	rightTextWindowPos = leftTextWindowPos + _screenSize.Width;

	gotoxy(LINE_NUMBER_OFFSET, _textPos.ScreenLine);

	for (i = 0; i < MAX_LINE_LENGTH; i++)
	{
		if (i >= leftTextWindowPos && i <= rightTextWindowPos)
		{
			if (i <= _textPos.LineLength)
			{
				cputc(_lineBuffer[i]);
			}
			else
			{
				cputc(' ');
			}
		}
	}

	gotoxy(
		_textPos.ScreenColumn + LINE_NUMBER_OFFSET,
		_textPos.ScreenLine);
}

void CursorLeft()
{
	if (_textPos.Column > 0)
	{
		_textPos.Column--;
		_textPos.ScreenColumn--;
		if (_textPos.ScreenColumn < 0)
		{
			// ScrollRight();
			_textPos.ScreenColumn = 0;
		}
	}
}

void CursorRight()
{
	if (_textPos.Column < _textPos.LineLength)
	{
		_textPos.Column++;
		_textPos.ScreenColumn++;
		if (_textPos.ScreenColumn > _screenSize.Width - 1)
		{
			// ScrollLeft();
			_textPos.ScreenColumn = _screenSize.Width - 1;
		}
	}
}

void InsertChar(char currentChar)
{
	unsigned char effectiveLineLength;

	if (_textPos.Column < MAX_LINE_LENGTH)
	{
		if (_textPos.LineLength == _textPos.Column)
		{
			_lineBuffer[_textPos.Column] = currentChar;
			_textPos.Column++;
			if (_textPos.ScreenColumn < _screenSize.Width)
			{
				cputc(currentChar);
				_textPos.ScreenColumn++;
			}
		}
		else
		{
			// We need to insert into the buffer and reprint and scroll the remainer of the line to the right.
			unsigned char i;
			effectiveLineLength=_textPos.LineLength;
			if (effectiveLineLength==MAX_LINE_LENGTH)
			{
				effectiveLineLength--;
			}

			for (i = effectiveLineLength; i >= _textPos.Column; i--)
			{
				_lineBuffer[i + 1] = _lineBuffer[i];
			}
			_lineBuffer[_textPos.Column] = currentChar;
			LineBufferToCurrentScreenLine();
		}

		// The line is getting longer in both cases.
		_textPos.LineLength++;
		CursorRight();
	}
}

void DeleteChar()
{
	unsigned char i;
	for (i = _textPos.Column; i < _textPos.LineLength; i++)
	{
		_lineBuffer[i + 1] = _lineBuffer[i];
	}

	_textPos.LineLength--;
	LineBufferToCurrentScreenLine();
}

void Backspace()
{
	if (_textPos.Column > 0)
	{
		CursorLeft();
		DeleteChar();
	}
	else
	{
		// TODO: Delete the Line and goto ende of previous line.
	}
}

void CursorUp()
{
	// TODO: Scrolling.
	SaveBufferToEditorMemory();
	_textPos.Line--;
	_textPos.ScreenLine--;
	GetBufferFromEditorMemoryAndPrint();
	gotoxy(_textPos.ScreenColumn, _textPos.ScreenLine);
}

void CursorDown()
{
	// We cannot go beyond the text end.
	if (_textPos.Line == _maxLine)
	{
		return;
	}

	// TODO: Scrolling.

	SaveBufferToEditorMemory();
	_textPos.Line++;
	_textPos.ScreenLine++;
	GetBufferFromEditorMemoryAndPrint();
	gotoxy(_textPos.ScreenColumn, _textPos.ScreenLine);
}

void HandleReturnKey()
{
	if (_textPos.Line == _maxLine)
	{
		SaveBufferToEditorMemory();
		_maxLine++;
		_textPos.Line++;
		_textPos.ScreenColumn = 0;
		_textPos.Column = 0;
		_textPos.LineLength = 0;
		if (_textPos.ScreenLine < _screenSize.Height)
		{
			_textPos.ScreenLine++;
		}
		else
		{
			Invalidate();
		}
		PrintLineNumber();
	}
	else
	{
		_textPos.ScreenColumn = 0;
		_textPos.Column = 0;
		CursorDown();
	}
}

void main(void)
{
	char currentChar;
	Initialize();
	PrintLineNumber();

	do
	{
		UpdateDocInfo(_textPos.Line, _textPos.Column, currentChar);

		currentChar = cgetc();

		if (currentChar == KEY_RETURN)
		{
			HandleReturnKey();
		}
		else if (currentChar == KEY_BACKSPACE)
		{
			Backspace();
		}
		else if (currentChar == KEY_CURDOWN)
		{
			CursorDown();
		}
		else if (currentChar == KEY_CURUP)
		{
			if (_textPos.ScreenLine > 0)
			{
				CursorUp();
			}
		}
		else if (currentChar == KEY_CURLEFT)
		{
			CursorLeft();
		}
		else if (currentChar == KEY_CURRIGHT)
		{
			CursorRight();
		}
		else
		{
			InsertChar(currentChar);
		}

	} while (currentChar != KEY_ESC);

	return;
}
