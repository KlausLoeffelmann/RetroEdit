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

void DebugPrint(const char *text, int value);
void DebugPrintSlow(const char *text, int value);
