/* Copyright INRIA */

#include <stdio.h>

extern int doit(int);
extern int doittoo(int);

int main()
{
  printf("doit(5) returns %d\n", doit(5));
  printf("doittoo(5) returns %d\n", doittoo(5));
  return 0;
}
