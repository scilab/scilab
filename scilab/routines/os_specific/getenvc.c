/* Copyright INRIA/ENPC */
/* returns setenv defined variable when getenv fails */
#include <stdio.h>
#include <string.h>

#include "../graphics/Math.h"
#include "Os_specific.h"

void C2F(getenvc)(int *ierr,char *var,char *buf,int *buflen,int *iflag)
{
  char *getenv(const char *),*local;
  *ierr=0;
#if defined(THINK_C)||defined(__MWERKS__)
  *buf=0;
#else
  if ( (local=getenv(var)) == 0)
    {
      if ( *iflag == 1 )
	sciprint("You must define the environment variable %s\r\n",var);
      *ierr=1;
      return;
    }
  else 
    {
      strncpy(buf,local,*buflen);
      *buflen = strlen(buf);
    }
#endif
  return;
}

