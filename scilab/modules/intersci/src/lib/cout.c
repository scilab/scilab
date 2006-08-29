/* Copyright INRIA */
#include <string.h>
#include "../machine.h"

extern void C2F(out)();

void Cout(str)
char *str;
{
  int l;
  l = strlen(str) + 1;
  C2F(out)(str,l);
}
