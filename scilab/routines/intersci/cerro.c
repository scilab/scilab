/* Copyright INRIA */
#include <string.h>
#include "../machine.h"
#include "cerro.h"

extern void C2F(erro)();

#define STRLEN 4096

void cerro(str)
char *str;
{
  int l;
  l = strlen(str) + 1;
  C2F(erro)(str,l);
}
