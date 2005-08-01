/* Copyright INRIA/ENPC */
#include "../machine.h"

#ifdef __STDC__
#include <stdlib.h>
#ifndef WIN32
#include <sys/types.h>
#include <unistd.h>
#endif
#else 
extern int getpid();
#endif

int C2F(getpidc)(int *id1)
{
  *id1=getpid();
  return(0) ;
}
