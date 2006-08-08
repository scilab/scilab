/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_SYMBOLIC__
#define __GW_SYMBOLIC__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "../machine.h"
#include "../stack-c.h"


typedef int (*Symbolic_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Symbolic_Interf f;    /** function **/
  char *name;      /** its name **/
} SymbolicTable;

#endif /*  __GW_SYMBOLIC__ */
/*-----------------------------------------------------------------------------------*/

