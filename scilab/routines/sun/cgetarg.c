#include <stdio.h>

#ifdef __STDC__
void cgetarg(int * n, char * str)
#else
void cgetarg(n,str)
  int * n;
  char * str;
#endif
{
#ifndef __ABSC__
  getarg(n,str,strlen(str));
#else
  GETARG(n,str,strlen(str));
#endif
}
