#include "machine.h"

void C2F(fcalc)(str,a) 
     char *str;
     int  *a;
{
  if ( strcmp(str,"one") ==0 )
    *a =1 ;
  else if ( strcmp(str,"two") ==0)
    *a =2;
  else 
    *a=-1;
}

