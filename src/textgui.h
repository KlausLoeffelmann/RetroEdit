typedef struct MenuItem MenuItem;
typedef struct PullDownMenu PullDownMenu;

struct MenuItem
{
	char *Text;
	unsigned char ItemStatus;
    unsigned char CtrlKeyboardShortCut;
    MenuItem *NextItem;
    MenuItem *SubItem;
    void *CallBack;
};

struct PullDownMenu
{
    MenuItem *TopLevelItems;
};

void InitPullDownMenu(PullDownMenu *pullDownMenu);