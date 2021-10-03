#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <cbm.h>

#include "screentools.h"

#define STACK_OFFSET_0 0

unsigned char PetAsciiToScreenCode(unsigned char petAscii)
{
            __asm__ ("ldy #%o", petAscii);
            __asm__ ("lda (sp),y");
            __asm__ ("tax");
            __asm__ ("inx");
            __asm__ ("txa");
            __asm__ ("sta (sp),y");
            return petAscii;
}

void DrawWindow(unsigned char column, unsigned char line, unsigned char width, unsigned char height, char *titel)
{
}
