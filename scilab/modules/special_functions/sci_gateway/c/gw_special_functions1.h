/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_SPECIAL_FUNCTIONS__
#define __GW_SPECIAL_FUNCTIONS__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "core_math.h"

typedef int (*Special_Functions_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Special_Functions_Interf f;    /** function **/
  char *name;      /** its name **/
} SpecialFunctionsTable;

#endif /*  __GW_SPECIAL_FUNCTIONS__ */
/*-----------------------------------------------------------------------------------*/

