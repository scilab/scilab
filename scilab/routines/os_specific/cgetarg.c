#include <stdio.h>

#ifdef __STDC__
void cgetarg(int * n, char * str)
#else
void cgetarg(n,str)
  int * n;
  char * str;
#endif
{
  getarg(n,str,strlen(str));
}
