#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <cbm.h>

#include "global.h"
#include "screentools.h"

void UpdateDocInfo(int line, int column, char currentChar)
{
	LineSegment *tmpLines = _editorLineSegments;
	gotoxy(1, _statusBarLineNo);
	printf("L: %05d", line);
	gotoxy(10, _statusBarLineNo);
	printf("C: %03d", column);
	gotoxy(20, _statusBarLineNo);
	printf("KC: %03d", currentChar);
	gotoxy(28, _statusBarLineNo);
	tmpLines += _maxLineSegment;
	printf("Ls*: %X/%X", tmpLines, _firstFreeSegment);

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

void DefinePullDownMenu()
{
	struct MenuItem *fileMenu, *fileNewProjectMenuItem, *fileOpenProjectMenuItem,
		*fileAddNewItemMenuItem, *fileSeperator1MenuItem, *fileSaveMenuItem,
		*fileSaveAsMenuItem, *fileSeperator2MenuItem, *fileExitMenuItem;

	struct MenuItem *editMenu, *editUndoMenuItem, *editRedoMenuItem, *editSeperator1MenuItem,
		*editCutMenuItem, *editCopyMenuItem, *editPasteMenuItem;

	// Defining File menu.
	unsigned char color=13;

	fileMenu = DefineMenuItem("&File", 0, 0, 0, color);
	fileNewProjectMenuItem = DefineMenuItem("&New project...", 0, 0, 0, color);
	fileOpenProjectMenuItem = DefineMenuItem("&Open project...", 0, 0, 0, color);
	fileAddNewItemMenuItem = DefineMenuItem("&Add new item...", 0, 0, "a", color);
	fileSeperator1MenuItem = DefineMenuItem("-", 0, 0, 0, color);
	fileSaveMenuItem = DefineMenuItem("Save file", 0, 0, "s", color);
	fileSaveAsMenuItem = DefineMenuItem("&Save file as...", 0, 0, 0, color);
	fileSeperator2MenuItem = DefineMenuItem("-", 0, 0, 0, color);
	fileExitMenuItem = DefineMenuItem("E&xit", 0, 0, 0, color);

	// Chaining File menu:
	fileMenu->SubItem = fileNewProjectMenuItem;
	fileNewProjectMenuItem->NextItem = fileOpenProjectMenuItem;
	fileOpenProjectMenuItem->NextItem = fileAddNewItemMenuItem;
	fileAddNewItemMenuItem->NextItem = fileSeperator1MenuItem;
	fileSeperator1MenuItem->NextItem = fileSaveMenuItem;
	fileSaveMenuItem->NextItem = fileSaveAsMenuItem;
	fileSaveAsMenuItem->NextItem = fileSeperator2MenuItem;
	fileSeperator2MenuItem->NextItem = fileExitMenuItem;
	fileExitMenuItem->NextItem = NULL;

	// Defining Edit menu.
	editMenu = DefineMenuItem("&Edit", 0, 0, 0, color);
	editUndoMenuItem = DefineMenuItem("&Undo", 0, 0, "z", color);
	editRedoMenuItem = DefineMenuItem("&Redo", 0, 0, "r", color);
	editSeperator1MenuItem = DefineMenuItem("-", 0, 0, 0, color);
	editCutMenuItem = DefineMenuItem("Cut", 0, 0, "x", color);
	editCopyMenuItem = DefineMenuItem("Copy", 0, 0, "c", color);
	editPasteMenuItem = DefineMenuItem("Pa&ste", 0, 0, "v", color);

	// Chaining of Edit-Menu:
	editMenu->SubItem = editUndoMenuItem;
	editUndoMenuItem->NextItem = editRedoMenuItem;
	editRedoMenuItem->NextItem = editSeperator1MenuItem;
	editSeperator1MenuItem->NextItem = editCutMenuItem;
	editCutMenuItem->NextItem = editCopyMenuItem;
	editCopyMenuItem->NextItem = editPasteMenuItem;
	editPasteMenuItem->NextItem = NULL;

	// Chaining of Top_level-Menus.
	fileMenu->NextItem = editMenu;
	editMenu->NextItem = NULL;

	_pullDownMenu.FirstTopLevelItem = fileMenu;
}

void Initialize()
{
	_maxLine = 0;
	_textPos.Column = 0;
	_textPos.Line = 0;
	_textPos.ScreenColumn = 0;
	_textPos.ScreenLine = 0;

	bgcolor(0);		// Black
	bordercolor(0); // Black
	textcolor(13);	// Green
	clrscr();

	DefinePullDownMenu();
	InitPullDownMenu(&_pullDownMenu);

	_editorLinesCapacity = START_EDITOR_LINES_CAPACITY;
	EnsureEditorLinesCapacity(1); // init:1

	screensize(&_screenSize.Width, &_screenSize.Height);
	_screenSize.EffectiveWidth = _screenSize.Width - LINE_NUMBER_OFFSET - 1;

	_screenSize.RightOffset = LINE_NUMBER_OFFSET - 1;
	_statusBarLineNo = _screenSize.Height - 1;
	_screenSize.FirstDocumentLine = FIRST_DOCUMENT_LINE;
	_screenSize.LastDocumentLine = _screenSize.Height - 2;
	_screenSize.EffectiveHeight = _screenSize.LastDocumentLine - _screenSize.FirstDocumentLine;
	_debugLineNo = _screenSize.Height - 2;

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

		if (i >= columnOffset && i <= (columnOffset + _screenSize.EffectiveWidth))
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

// Repaints the whole screen - optimzed version.
// We have access to the screen memory, and can optimze a lot!
void Invalidate(char *lineBuffer)
{
	unsigned char i, line, startLine, endLine, currentLineLength;
	unsigned char leftTextWindowPos, rightTextWindowPos;

	// We access the screen memory directly and will NOT use cput.
	unsigned char *screenMem, *workingLineBuffer;

	screenMem = (unsigned char *)SCREENMEM + _screenSize.Width * _screenSize.FirstDocumentLine + LINE_NUMBER_OFFSET;

	leftTextWindowPos = _textPos.Column - _textPos.ScreenColumn;
	rightTextWindowPos = leftTextWindowPos + _screenSize.EffectiveWidth;

	startLine = _textPos.Line - _textPos.ScreenLine;
	endLine = startLine + _screenSize.EffectiveHeight;

	for (line = startLine; line < endLine; line++)
	{
		if (line <= _maxLine)
		{
			// Print line number.
			gotoxy(0, line + _screenSize.FirstDocumentLine);
			printf("%04d!", line + 1);

			gotoxy(LINE_NUMBER_OFFSET, line + _screenSize.FirstDocumentLine);

			if (lineBuffer != NULL && line == _textPos.ScreenLine)
			{
				workingLineBuffer = lineBuffer;
				currentLineLength = _textPos.LineLength;
			}
			else
			{
				currentLineLength = GetWorkingLine(line);
				workingLineBuffer = _workingLineBuffer;
			}

			for (i = leftTextWindowPos; i <= rightTextWindowPos; i++)
			{
				*screenMem++ = PetAsciiToScreenCode(workingLineBuffer[i]);
			}
		}
		else
		{
			for (i = leftTextWindowPos; i <= rightTextWindowPos; i++)
			{
				*screenMem++ = SPACE;
			}
		}

		screenMem = screenMem + LINE_NUMBER_OFFSET;
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
	rightTextWindowPos = leftTextWindowPos + _screenSize.EffectiveWidth;

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
			_textPos.ScreenColumn = 0;
			Invalidate(NULL);
		}
	}
}

void CursorRight()
{
	if (_textPos.Column < _textPos.LineLength)
	{
		_textPos.Column++;
		_textPos.ScreenColumn++;
		if (_textPos.ScreenColumn > _screenSize.EffectiveWidth - 1)
		{
			_textPos.ScreenColumn = _screenSize.EffectiveWidth - 1;
			Invalidate(NULL);
		}
	}
}

void DeleteLine(unsigned int lineNumber)
{
	LineSegment *tmpLineSegment;
	register unsigned char i;

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
	Invalidate(NULL);
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
			if (_textPos.ScreenColumn <= _screenSize.EffectiveWidth - 1)
			{
				cputc(currentChar);
				_textPos.ScreenColumn++;
			}
			else
			{ // We invalidate everything, and pass the current lineBuffer.
				// Invalidate then uses this buffer instead of the getting
				// the line from memory in a temporary buffer for the current line.
				Invalidate(_lineBuffer);
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
	if (_textPos.Line == 0U)
	{
		return;
	}

	SaveBufferToEditorMemory();
	_textPos.Line--;

	if (_textPos.ScreenLine >= _screenSize.FirstDocumentLine)
	{
		_textPos.ScreenLine--;
	}
	else
	{
		Invalidate(NULL);
	}

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

	SaveBufferToEditorMemory();
	_textPos.Line++;
	if (_textPos.ScreenLine <= _screenSize.EffectiveHeight)
	{
		_textPos.ScreenLine++;
	}
	else
	{
		Invalidate(NULL);
	}

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
			Invalidate(NULL);
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

	// ClearScreenEx(5, 3, 30, 10, 'A');
	// ClearScreen();
	// DrawWindow(5, 3, 20, 15, 42);
	// return;

	Initialize();
	PrintLineNumber();

	do
	{
		UpdateDocInfo(_textPos.Line, _textPos.Column, currentChar);
		currentChar = cgetc();

		HandlePullDownMenu(&_pullDownMenu, currentChar);

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
