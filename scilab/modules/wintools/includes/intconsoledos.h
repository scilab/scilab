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

#ifndef NULL
  #define NULL 0
#endif

#define TRUE  1
#define FALSE 0

int C2F(intconsoledos) _PARAMS((char *fname));

#endif /* __INTCONSOLEDOS__ */
/*-----------------------------------------------------------------------------------*/ 
