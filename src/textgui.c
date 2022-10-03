#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <cbm.h>

#include "screentools.h"
#include "textgui.h"

#define PULLDOWNMENU_BACKGROUNDCOLOR 0
#define INITIAL_LISTBOX_ITEMCOUNT 4

unsigned char GetAcceleratorKey(char *text)
{
    unsigned char index = 0;
    unsigned char currentChar;

    for (;;)
    {
        currentChar = text[index];

        if (currentChar == NULL)
            break;

        if (currentChar == '&')
        {
            ++index;
            return text[index];
        }

        ++index;
    }
}

MenuItem *DefineMenuItem(
    char *text,
    unsigned char id,
    unsigned char itemStatus,
    unsigned char ctrlKeyboardShortCut)
{
    ListItem *listItem;
    MenuItem *menuItem;

    listItem = malloc(sizeof(ListItem));
    menuItem = malloc(sizeof(MenuItem));

    listItem->Text = text;
    listItem->ID_ListItem = id;
    listItem->CtrlKeyboardShortCut = ctrlKeyboardShortCut;

    menuItem->ListItem = listItem;

    return menuItem;
}

void InitPullDownMenu(PullDownMenu *pullDownMenu)
{
    register MenuItem *topLevelItem = pullDownMenu->FirstTopLevelItem;

    unsigned char x;
    unsigned char length;

    x = 2;
    length = 0;

    while (topLevelItem != NULL)
    {
        register unsigned char *menuText = topLevelItem->ListItem->Text;
        length = DrawUIText(menuText, x, 0, PULLDOWNMENU_BACKGROUNDCOLOR);
        topLevelItem->ListItem->AcceleratorKey = GetAcceleratorKey(menuText);
        x = x + length + 2;
        topLevelItem = topLevelItem->NextItem;
    }

    DrawHLine(0, 1, 40, CHAR_HORIZONTAL_LINE);
}

void HandlePullDownMenu(PullDownMenu *pullDownMenu, char pressedKey)
{
    register MenuItem *menuItem = pullDownMenu->FirstTopLevelItem;
    register MenuItem *subMenuItem;

    unsigned char handled = 0;

    while (menuItem != NULL)
    {
        if (menuItem->ListItem->AcceleratorKey == pressedKey)
        {
            OpenMenu(menuItem);
            break;
        }

        menuItem = menuItem->NextItem;
    }
}

Listbox *InitList(char *title)
{
    Listbox *listbox;
    ListItem *listItem;
    ListItem *testArray;

    listbox = malloc(sizeof(Listbox));
    listbox->Count = 0;
    listbox->ReservedCount = 4;
    listbox->ListItems = malloc(sizeof(ListItem) * 4);

    return listbox;
}

void AddListItem(Listbox *listbox, ListItem *listItem)
{
    int capacity;
    capacity = listbox->ReservedCount;

    if (listbox->Count == capacity)
    {
        // Ensure capacity.
        capacity += capacity;
        listbox->ListItems = realloc(listbox->ListItems, sizeof(ListItem) * capacity);
        listbox->ReservedCount = capacity;
    }

    listbox->ListItems[listbox->Count] = *listItem;
}

void OpenMenu(MenuItem *menuItem)
{
    MenuItem *currentMenuItem;
    Listbox *listbox;
    unsigned char maxTextLength;

    if (menuItem->SubItem == NULL)
    {
        // We have no sub items, we return.
        return;
    }

    listbox=InitList(NULL);
    currentMenuItem = menuItem->SubItem;

    // Convert the menu items to a listbox:
    while (currentMenuItem != NULL)
    {
        ListItem *listItem, *menuListItem;
        unsigned char currentTextLength;

        listItem = malloc(sizeof(ListItem));
        menuListItem = currentMenuItem->ListItem;
        listItem->ID_ListItem = menuListItem->ID_ListItem;
        listItem->Text = menuListItem->Text;
        listItem->ItemStatus = menuListItem->ItemStatus;
        listItem->AcceleratorKey=menuListItem->AcceleratorKey;
        listItem->CtrlKeyboardShortCut=menuListItem->CtrlKeyboardShortCut;
    }
}
