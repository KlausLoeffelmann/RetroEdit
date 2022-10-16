#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <cbm.h>

#include "screentools.h"
#include "textgui.h"

#define PULLDOWNMENU_BACKGROUNDCOLOR 0
#define INITIAL_LISTBOX_ITEMCOUNT 4

unsigned char _acceleratorKeyLookup[] = {
    176, 191, 188, 177, 187, 165, 180,
    162, 181, 161, 182, 167, 170, 185,
    175, 171, 178, 174, 163, 184, 190,
    179, 189, 183, 173};

TextWindow *DefineTextWindow(unsigned char line, unsigned char col, unsigned char width, unsigned char height)
{
    TextWindow *textWindow;

    DrawWindow(col, line, width, height, 0);

    textWindow = malloc(sizeof(TextWindow));
    textWindow->Line = line;
    textWindow->Column = col;
    textWindow->Width = width;
    textWindow->Height = height;
    textWindow->Handle = 0;
    textWindow->CurrentColumn = 0;
    textWindow->CurrentLine = 0;

    return textWindow;
}

void TextWindowPrint(TextWindow *textWindow, register char *text)
{
    register int *screenMemory;
    unsigned char maxCol, maxLine;

    screenMemory = (int *)SCREEN_MEMORY;
    maxCol = textWindow->Column + textWindow->Height - 2;
    maxLine = textWindow->Line + textWindow->Width - 2;

    screenMemory = screenMemory +
                   textWindow->CurrentLine * MAX_SCREEN_WIDTH +
                   textWindow->CurrentColumn;

    do
    {
        *screenMemory = *text;
        ++text;

        ++textWindow->CurrentColumn;
        if (textWindow->CurrentColumn > maxCol)
        {
            textWindow->CurrentColumn = textWindow->Column + 1;
            ++textWindow->CurrentLine;

            if (textWindow->CurrentLine > maxLine)
            {
                // ScrollUp!
                --textWindow->CurrentLine;
            }

            screenMemory = screenMemory +
                           textWindow->CurrentLine * MAX_SCREEN_WIDTH +
                           textWindow->CurrentColumn;
        }
        else
        {
            ++screenMemory;
        }

    } while (*text != 0);
}

void InitPullDownMenu(PullDownMenu *pullDownMenu)
{
    MenuItem *topLevelItem = pullDownMenu->FirstTopLevelItem;

    unsigned char x, accKey;
    unsigned char length;

    x = 2;
    length = 0;

    while (topLevelItem != NULL)
    {
        unsigned char *menuText = topLevelItem->ListItem->Text;
        length = DrawUIText(menuText, x, 0, PULLDOWNMENU_BACKGROUNDCOLOR);
        accKey = GetAcceleratorKey(menuText);
        topLevelItem->ListItem->AcceleratorKey = accKey;
        x = x + length + 2;
        topLevelItem = topLevelItem->NextItem;
    }

    DrawHLine(0, 1, 40, CHAR_HORIZONTAL_LINE);
}

unsigned char GetAcceleratorKey(register char *text)
{
    register unsigned char index = 0;
    register unsigned char currentChar;

    for (;;)
    {
        currentChar = text[index];

        if (currentChar == NULL)
            break;

        if (currentChar == '&')
        {
            ++index;
            currentChar = _acceleratorKeyLookup[text[index] - 193];
            return currentChar;
        }

        ++index;
    }

    return 0;
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
    ++listbox->Count;
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

    listbox = InitList(NULL);
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
        listItem->AcceleratorKey = menuListItem->AcceleratorKey;
        listItem->CtrlKeyboardShortCut = menuListItem->CtrlKeyboardShortCut;

        AddListItem(listbox, listItem);

        currentMenuItem = currentMenuItem->NextItem;
    }
}
