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
    //a   b    c    d    e    f    g  
    176, 191, 188, 172, 177, 187, 165, 
    //h   i    j    k    l    m    n
    180, 162, 181, 161, 182, 167, 170, 
    //o   p    q    r    s    t    u
    185, 175, 171, 178, 174, 163, 184, 
    //v   w    x    y    z
    190, 179, 189, 183, 173};

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
        topLevelItem->Left = x;
        length = DrawUIText(menuText, x, 0, topLevelItem->ListItem->Color);
        x = x + length + 2;
        topLevelItem = topLevelItem->NextItem;
    }

    DrawHLine(0, 1, 40, CHAR_HORIZONTAL_LINE);
}

unsigned char GetAcceleratorKeyAndLength(register char *text, unsigned char *length)
{
    register unsigned char index = 0;
    register unsigned char currentChar;
    unsigned char accKey;

    accKey = 0;

    for (;;)
    {
        currentChar = text[index];

        if (currentChar == '&')
        {
            ++index;
            accKey = _acceleratorKeyLookup[text[index] - 193];
            currentChar = text[index];
        }

        if (currentChar == NULL) break;

        ++index;
    }

    if (accKey > 0)
    {
        --index;
    }

    *length = index;
    return accKey;
}

MenuItem *DefineMenuItem(
    char *text,
    unsigned char id,
    unsigned char itemStatus,
    unsigned char ctrlKeyboardShortCut,
    unsigned char color)
{
    ListItem *listItem;
    MenuItem *menuItem;

    unsigned char length, accKey;
    length = 42;

    listItem = malloc(sizeof(ListItem));
    menuItem = malloc(sizeof(MenuItem));

    listItem->Text = text;
    listItem->ID_ListItem = id;
    listItem->CtrlKeyboardShortCut = ctrlKeyboardShortCut;
    listItem->Color = color;
    accKey = GetAcceleratorKeyAndLength(text, &length);

    listItem->AcceleratorKey = accKey;
    listItem->TextLength = length;

    menuItem->ListItem = listItem;
    return menuItem;
}

void HandlePullDownMenu(PullDownMenu *pullDownMenu, char pressedKey)
{
    register MenuItem *menuItem = pullDownMenu->FirstTopLevelItem;
    register MenuItem *subMenuItem;

    unsigned char handled = 0;

    // printf("\n&pullDM:%x - &pressedKey:%x\n", pullDownMenu, &pressedKey);
    // printf("------------------------------------------------------\n");

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

Listbox *InitList(char *title, unsigned char color)
{
    Listbox *listbox;
    ListItem *listItem;
    ListItem *testArray;

    listbox = malloc(sizeof(Listbox));

    listbox->Count = 0;
    listbox->ReservedCount = 4;
    listbox->ListItems = malloc(sizeof(ListItem) * 4);
    listbox->Color = color;

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

    // Calculate width of the ListBox
    if (listbox->Width < listItem->TextLength)
    {
        listbox->Width = listItem->TextLength;
    }
    
    ++listbox->Count;
}

unsigned char OpenMenu(MenuItem *menuItem)
{
    MenuItem *currentMenuItem;
    Listbox *listbox;
    unsigned char maxTextLength;
    char currentChar;

    cursor(0);

    if (menuItem->SubItem == NULL)
    {
        // We have no sub items, we return.
        return NULL;
    }

    listbox = InitList(NULL, menuItem->ListItem->Color);
    currentMenuItem = menuItem->SubItem;

    // Convert the menu items to a listbox:
    while (currentMenuItem != NULL)
    {
        AddListItem(listbox, currentMenuItem->ListItem);
        currentMenuItem = currentMenuItem->NextItem;
    }

    listbox->Left = menuItem->Left;
    listbox->Top = 1;
    listbox->MaxHeight = 20;
    currentChar = HandleListBox(listbox);
    cursor(1);

    return currentChar;
}

unsigned char HandleListBox(Listbox *listbox)
{
    unsigned char height, color, i, left, currentLine, exit;
    ListItem currentListItem;
    unsigned char currentChar;

    if (listbox->ListItems == NULL)
    {
        return NULL;
    }

    if (listbox->Count <= listbox->MaxHeight)
    {
        height = listbox->Count;
    }
    else
    {
        height = listbox->MaxHeight;
    }

    ++height;
    left = listbox->Left;

    DrawWindow(left, listbox->Top, listbox->Width + 3, height, listbox->Color);
    left += 2;

    for (i = 0; i < listbox->Count; i++)
    {
        currentListItem = listbox->ListItems[i];
        DrawUIText(currentListItem.Text, left, i + 2, currentListItem.Color);
    }

    // Main control loop
    currentListItem = listbox->ListItems[0];
    i = 0;
    exit = 0;

    do
    {
        // Draw the Item invers:
        DrawUIText(currentListItem.Text, left, i + 2, currentListItem.Color + 0x80);
        currentChar = cgetc();
        DrawUIText(currentListItem.Text, left, i + 2, currentListItem.Color);

        if (currentChar == KEY_CURDOWN)
        {
            ++i;
            if (i > listbox->Count)
            {
                i = listbox->Count;
            }
        }
        else if (currentChar == KEY_CURUP)
        {
            if (i > 0)
            {
                --i;
            }
        }
        else
        {
            exit = 1;
        }

        currentListItem = listbox->ListItems[0];

    } while (exit == 0);
}
