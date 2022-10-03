typedef struct ListItem ListItem;
typedef struct Listbox Listbox;
typedef struct MenuItem MenuItem;
typedef struct PullDownMenu PullDownMenu;

#define ITEMSTATUS_ENABLED = 1;
#define ITEMSTATUS_CHECKED = 2;
#define ITEMSTATUS_VISIBLE = 4;
#define ITEMSTATUS_HIGHLIGTHED = 8;

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

void InitPullDownMenu(PullDownMenu *pullDownMenu);
void HandlePullDownMenu(PullDownMenu *pullDownMenu, char pressedKey);
void HandlePullDownMenu(PullDownMenu *pullDownMenu, char pressedKey);
Listbox* InitList(char *title);
void AddListItem(Listbox *listbox, ListItem *listItem);
void OpenMenu(MenuItem *menuItem);
MenuItem *DefineMenuItem(char *text, unsigned char id, unsigned char itemStatus, unsigned char ctrlKeyboardShortCut);
