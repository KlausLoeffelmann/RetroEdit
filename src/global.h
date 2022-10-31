#include "textgui.h" 

#define SEGMENT_TEXT_LENGTH 10
#define START_EDITOR_LINES_CAPACITY 4

#define MAX_LINE_LENGTH 160
#define LINE_NUMBER_OFFSET 6
#define FIRST_DOCUMENT_LINE 2

#if __CX16__
	#define EDITOR_MEMORY 0x9000
#else
	#define EDITOR_MEMORY 0xC000
#endif

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
	unsigned char EffectiveWidth;
	unsigned char EffectiveHeight;
	unsigned char Height;
	unsigned char Width;
	unsigned char FirstDocumentLine;
	unsigned char LastDocumentLine;
	unsigned char RightOffset;
};

struct TextPosition
{
	unsigned int Line;
	unsigned char Column;
	unsigned char ScreenLine;
	unsigned char ScreenColumn;
	unsigned char LineLength;
	unsigned char *Text;
};

// TODO: We need to correct this for CX16 and Apple.
#if __C64__
	#define SCREENMEM 0x0400
#else
	#define SCREENMEM 0x0A00
#endif

void DebugPrint(const char *text, int value);
void DebugPrintSlow(const char *text, int value);

// *** Global variables, defined in Global.c ***

// The current line buffer.
// A line has 255 chars max. When it's time to commit the line,
// it's getting transferred into segments of SEGMENT_TEXT_LENGTH
// Bytes.
extern char _lineBuffer[MAX_LINE_LENGTH];
extern char _workingLineBuffer[MAX_LINE_LENGTH];

// Pointer to the array holding the point to the start
// and the length of each line.
extern EditorLine *_editorLines;

// Count of EditorLine pointers.
extern unsigned int _editorLinesCapacity;

// The pointer to the EditorLineSegments holding
// the actual text in memory.
extern LineSegment *_editorLineSegments;

// This does not point unconditionally to the next available Segment,
// but point only to a segment, which is no longer in use (so has been freed).
extern LineSegment *_firstFreeSegment;

extern unsigned int _maxLineSegment;

// The current TextPosition.
extern TextPosition _textPos;

// The current screensize.
extern ScreenSize _screenSize;

// The line number in which the status bar is at.
extern unsigned char _statusBarLineNo;
extern unsigned char _debugLineNo;

// The max lines we can use.
extern unsigned int _maxLine;

extern PullDownMenu _pullDownMenu;
