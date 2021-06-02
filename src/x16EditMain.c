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
#define KEY_DELETE 148
#define KEY_ERASELINE 5
#define KEY_TAB 9

#define SEGMENT_TEXT_LENGTH 10
#define START_EDITOR_LINES_CAPACITY 4

#define MAX_LINE_LENGTH 255
#define LINE_NUMBER_OFFSET 6

#define EDITOR_MEMORY 0x9000

#define SPACE 32

// A Segment of SEGMENT_TEXT_LENGTH of bytes for storing text.
typedef struct LineSegment LineSegment;

// The current TextPosition.
typedef struct TextPosition TextPosition;

// The Size of the Screen.
typedef struct ScreenSize ScreenSize;

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

struct ScreenSize
{
	unsigned char Width;
	unsigned char Height;
	unsigned char FirstDocumentLine;
	unsigned char LastDocumentLine;
	unsigned char EffectiveScreenHeight;
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
char _workingLineBuffer[255];

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
		_textPos.ScreenLine + _screenSize.FirstDocumentLine);
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

	_editorLinesCapacity = START_EDITOR_LINES_CAPACITY;
	EnsureEditorLinesCapacity(1); // init:1

	screensize(&_screenSize.Width, &_screenSize.Height);

	_screenSize.Width = _screenSize.Width - LINE_NUMBER_OFFSET - 1;
	_statusBarLineNo = _screenSize.Height - 1;
	_screenSize.FirstDocumentLine = 1;
	_screenSize.LastDocumentLine = _screenSize.Height - 2;
	_screenSize.EffectiveScreenHeight = _screenSize.LastDocumentLine - _screenSize.FirstDocumentLine;
	_debugLineNo = _screenSize.Height - 2;

	clrscr();
	cursor(1);

	_maxLineSegment = 0;
	_editorLineSegments = (LineSegment *)EDITOR_MEMORY;
	_firstFreeSegment = NULL;

	gotoxy(LINE_NUMBER_OFFSET,
		   _textPos.ScreenLine + _screenSize.FirstDocumentLine);
}

void PrintLineNumber()
{
	gotoxy(0, _textPos.ScreenLine + _screenSize.FirstDocumentLine);
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
	}

	return nextFreeSegment;
}

void SaveBufferToEditorMemory()
{
	unsigned char y;
	unsigned char i;

	LineSegment *nextLineSegment, *currentLineSegment;

	// We need to expand the collection with LineStart pointers.
	EnsureEditorLinesCapacity(0);
	currentLineSegment = GetNextFreeLineSegment();
	_editorLines[_textPos.Line].firstLineSegment = currentLineSegment;

	y = 0;
	for (i = 0; i < _textPos.LineLength; i++)
	{
		if (y == SEGMENT_TEXT_LENGTH)
		{
			nextLineSegment = GetNextFreeLineSegment();
			currentLineSegment->nextSegment = nextLineSegment;
			currentLineSegment = nextLineSegment;
			y = 0;
		}
		currentLineSegment->text[y++] = _lineBuffer[i];
	}
	_editorLines[_textPos.Line].length = _textPos.LineLength;
	currentLineSegment->nextSegment = NULL;
}

void GetBufferFromEditorMemoryAndPrint(unsigned char suppressFreeMem)
{
	LineSegment *currentLineSegment, *tmpLineSegment;
	unsigned char lineLength;
	unsigned char charpoint;
	unsigned char i;
	unsigned char columnOffset;

	if (suppressFreeMem == 0)
	{
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
		else
		{
			// If we adding new lines, this will stay NULL. If we move around and edit,
			// this leads to reusing the existing segments.
			_firstFreeSegment = _editorLines[_textPos.Line].firstLineSegment;
		}
	}

	columnOffset = _textPos.Column - _textPos.ScreenColumn;
	currentLineSegment = _editorLines[_textPos.Line].firstLineSegment;
	lineLength = _editorLines[_textPos.Line].length;
	_textPos.LineLength = lineLength;
	charpoint = 0;

	gotoxy(LINE_NUMBER_OFFSET, _textPos.ScreenLine + _screenSize.FirstDocumentLine);

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

// Copy the whole segment chain of a line into the working line buffer
// and returns the length of that line.
unsigned int GetWorkingLine(unsigned int lineNumber)
{
	unsigned char i, lineLength, charpoint;
	LineSegment *currentLineSegment;

	currentLineSegment = _editorLines[lineNumber].firstLineSegment;
	lineLength = _editorLines[lineNumber].length;
	charpoint = 0;

	for (i = 0; i < MAX_LINE_LENGTH; i++)
	{
		if (i < lineLength)
		{
			_workingLineBuffer[i] = currentLineSegment->text[charpoint++];
			if (charpoint == SEGMENT_TEXT_LENGTH)
			{
				currentLineSegment = currentLineSegment->nextSegment;
				charpoint = 0;
			}
		}
		else
		{
			_workingLineBuffer[i] = 32;
		}
	}

	return lineLength;
}

// Repaints the whole screen.
void Invalidate()
{
	unsigned char i, line, startLine, endLine, currentLineLength;
	unsigned char leftTextWindowPos, rightTextWindowPos;

	leftTextWindowPos = _textPos.Column - _textPos.ScreenColumn;
	rightTextWindowPos = leftTextWindowPos + _screenSize.Width;

	startLine = _textPos.Line - _textPos.ScreenLine;
	endLine = startLine + _screenSize.EffectiveScreenHeight;

	for (line = startLine; line < endLine; line++)
	{
		if (line <= _maxLine)
		{
			// Print line number.
			gotoxy(0, line + _screenSize.FirstDocumentLine);
			printf("%04d:", line + 1);

			gotoxy(LINE_NUMBER_OFFSET, line + _screenSize.FirstDocumentLine);

			if (line < _maxLine)
			{
				currentLineLength = GetWorkingLine(line);

				for (i = 0; i < MAX_LINE_LENGTH; i++)
				{
					if (i >= leftTextWindowPos && i <= rightTextWindowPos)
					{
						cputc(_workingLineBuffer[i]);
					}
				}
			}
		}
		else
		{
			for (i = leftTextWindowPos; i <= rightTextWindowPos; i++)
			{
				cputc(SPACE);
			}
		}
	}

	gotoxy(
		_textPos.ScreenColumn + LINE_NUMBER_OFFSET,
		_textPos.ScreenLine + _screenSize.FirstDocumentLine);

	_textPos.LineLength = _editorLines[_textPos.Line].length;
}

void LineBufferToCurrentScreenLine()
{
	unsigned char i;
	unsigned char leftTextWindowPos, rightTextWindowPos;

	leftTextWindowPos = _textPos.Column - _textPos.ScreenColumn;
	rightTextWindowPos = leftTextWindowPos + _screenSize.Width;

	gotoxy(LINE_NUMBER_OFFSET, _textPos.ScreenLine + _screenSize.FirstDocumentLine);

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
		_textPos.ScreenLine + _screenSize.FirstDocumentLine);
}

void CursorLeft()
{
	if (_textPos.Column > 0)
	{
		_textPos.Column--;
		_textPos.ScreenColumn--;

		// This is would be an underflow.
		if (_textPos.ScreenColumn == 255)
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

void DeleteLine(unsigned int lineNumber)
{
	LineSegment *tmpLineSegment;
	unsigned char i;

	if (_maxLine == 0)
	{
		return;
	}

	if (_maxLine < lineNumber)
	{
		return;
	}

	// Free segments used by that line:
	if (_firstFreeSegment != NULL)
	{
		// We need to find the end of the free segments, and ...
		tmpLineSegment = _firstFreeSegment;
		while (tmpLineSegment->nextSegment != NULL)
		{
			tmpLineSegment = tmpLineSegment->nextSegment;
		}

		// ...chain it with the segments of the line to delete.
		tmpLineSegment->nextSegment = _editorLines[lineNumber].firstLineSegment;
	}
	else
	{
		// If there weren't any free segments before - now we have them!
		_firstFreeSegment = _editorLines[lineNumber].firstLineSegment;
	}

	// Now, let's delete the pointer.
	for (i = lineNumber; i < _maxLine; i++)
	{
		_editorLines[i] = _editorLines[i + 1];
	}

	_maxLine--;
	_textPos.LineLength = _editorLines[_textPos.Line].length;
	Invalidate();
	GetBufferFromEditorMemoryAndPrint(1);
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
			effectiveLineLength = _textPos.LineLength;
			if (effectiveLineLength == MAX_LINE_LENGTH)
			{
				effectiveLineLength--;
			}

			for (i = effectiveLineLength;; i--)
			{
				_lineBuffer[i + 1] = _lineBuffer[i];

				// The underflow can't be handled correctly inside a for loop.
				if (i == _textPos.Column)
				{
					break;
				}
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

	if (_textPos.Column < _textPos.LineLength)
	{

		for (i = _textPos.Column; i < _textPos.LineLength; i++)
		{
			_lineBuffer[i] = _lineBuffer[i + 1];
		}

		_lineBuffer[_textPos.LineLength - 1] = ' ';
		_textPos.LineLength--;
		LineBufferToCurrentScreenLine();
	}
	else
	{
		// Delete line, if not in last line.
	}
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
	_textPos.LineLength = (_editorLines + _textPos.Line)->length;
	GetBufferFromEditorMemoryAndPrint(/* supressFreeMem */ 0);
	gotoxy(_textPos.ScreenColumn, _textPos.ScreenLine + _screenSize.FirstDocumentLine);
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
	_textPos.LineLength = (_editorLines + _textPos.Line)->length;
	GetBufferFromEditorMemoryAndPrint(/* supressFreeMem */ 0);
	gotoxy(_textPos.ScreenColumn, _textPos.ScreenLine + _screenSize.FirstDocumentLine);
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
		else if (currentChar == KEY_DELETE)
		{
			DeleteChar();
		}
		else if (currentChar == KEY_ERASELINE)
		{
			if (_textPos.Line < _maxLine)
			{
				DeleteLine(_textPos.Line);
			}
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
