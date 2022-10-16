typedef struct TextWindow TextWindow;
typedef struct ListItem ListItem;
typedef struct Listbox Listbox;
typedef struct MenuItem MenuItem;
typedef struct PullDownMenu PullDownMenu;

#define ITEMSTATUS_ENABLED 1;
#define ITEMSTATUS_CHECKED 2;
#define ITEMSTATUS_VISIBLE 4;
#define ITEMSTATUS_HIGHLIGTHED 8;

#define MAX_SCREEN_WIDTH 40;
#define MAX_SCREEN_HEIGHT 25;
#define SCREEN_MEMORY 1024;

struct TextWindow
{
    unsigned char Handle;
    unsigned char Line;
    unsigned char Column;
    unsigned char Width;
    unsigned char Height;
    unsigned char CurrentLine;
    unsigned char CurrentColumn;
};

struct ListItem
{
    char *Text;
    unsigned char ID_ListItem;
    unsigned char ItemStatus;

    // We need this to be compatible to MenuItems.
    unsigned char CtrlKeyboardShortCut;
    unsigned char AcceleratorKey;
};

struct Listbox
{
    unsigned char Count;
    unsigned char ReservedCount;
    unsigned char Title;
    void *CallBack;
    ListItem* ListItems;
};

struct MenuItem
{
    ListItem *ListItem;
    MenuItem *NextItem;
    MenuItem *SubItem;
    void *CallBack;
};

struct PullDownMenu
{
    MenuItem *FirstTopLevelItem;
};

TextWindow *DefineTextWindow(unsigned char line, unsigned char col, unsigned char width, unsigned char height);
void InitPullDownMenu(PullDownMenu *pullDownMenu);
unsigned char GetAcceleratorKey(register char *text);
void HandlePullDownMenu(PullDownMenu *pullDownMenu, char pressedKey);
void HandlePullDownMenu(PullDownMenu *pullDownMenu, char pressedKey);
Listbox* InitList(char *title);
void AddListItem(Listbox *listbox, ListItem *listItem);
void OpenMenu(MenuItem *menuItem);
MenuItem *DefineMenuItem(char *text, unsigned char id, unsigned char itemStatus, unsigned char ctrlKeyboardShortCut);
