#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <cbm.h>

unsigned int someVar;
unsigned char charset[255][16];

void SomeMethod()
{
	printf("%04d:", 1);
	charset[0][0] = 0b10001000;
}
