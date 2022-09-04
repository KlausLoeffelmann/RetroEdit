#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <cbm.h>

#include "screentools.h"
#include "textgui.h"

#define PULLDOWNMENU_BACKGROUNDCOLOR 0


void InitPullDownMenu(PullDownMenu *pullDownMenu)
{
    register MenuItem *topLevelItem = pullDownMenu->TopLevelItems;

    unsigned char x;
    unsigned char length;

    x = 2;
    length = 0;

    while (topLevelItem != NULL)
    {
        length = DrawUIText(topLevelItem->Text, x, 0, PULLDOWNMENU_BACKGROUNDCOLOR);
        x = x + length + 2;
        topLevelItem = topLevelItem->NextItem;
    }

    DrawHLine(0, 1, 40, CHAR_HORIZONTAL_LINE);
}
