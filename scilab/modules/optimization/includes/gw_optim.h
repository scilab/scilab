/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_OPTIM__
#define __GW_OPTIM__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"


typedef int (*Optim_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Optim_Interf f;    /** function **/
  char *name;      /** its name **/
} OptimTable;

#endif /*  __GW_OPTIM__ */
/*-----------------------------------------------------------------------------------*/

