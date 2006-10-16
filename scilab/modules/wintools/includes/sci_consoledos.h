/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTCONSOLEDOS__
#define __INTCONSOLEDOS__

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "version.h"

#ifdef _MSC_VER
#include "WinConsole.h"
#endif

int C2F(sci_consoledos) _PARAMS((char *fname,unsigned long l));

#endif /* __INTCONSOLEDOS__ */
/*-----------------------------------------------------------------------------------*/ 
