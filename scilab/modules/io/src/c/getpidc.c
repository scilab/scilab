/* Copyright INRIA/ENPC */
#include "machine.h"

#include <stdlib.h>
#ifdef _MSC_VER
	#include <process.h>
	#define getpid _getpid
#else
	#include <sys/types.h>
	#include <unistd.h>
#endif
#include "getpidc.h"

int C2F(getpidc)(int *id1)
{
  *id1=getpid();
  return(0) ;
}
