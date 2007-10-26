/* Copyright INRIA */
#include <string.h>
#include "machine.h"
#include "cerro.h"

void cerro(char *str)
{
  int l;
  l = strlen(str) + 1;
  C2F(erro)(str,l);
}
