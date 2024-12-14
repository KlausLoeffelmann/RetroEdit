#include <stdio.h>

int main()
{
  register unsigned char *poke;

  for (poke = (unsigned char *)1024; poke < (unsigned char *)(1024 + 1000); poke++)
  {
    *poke = 65;
  }
  return 0;
}
