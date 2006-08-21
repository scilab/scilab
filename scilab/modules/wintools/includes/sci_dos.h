/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTDOS__
#define __INTDOS__

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "version.h"

int C2F(sci_dos) _PARAMS((char *fname,unsigned long l));

#endif /* __INTDOS__ */
/*-----------------------------------------------------------------------------------*/ 
