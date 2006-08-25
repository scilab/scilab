/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_ELEMENTARIES_FUNCTIONS__
#define __GW_ELEMENTARIES_FUNCTIONS__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"


typedef int (*Elementaries_Functions_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Elementaries_Functions_Interf f;    /** function **/
  char *name;      /** its name **/
} ElementariesFunctionsTable;

#endif /*  __GW_ELEMENTARIES_FUNCTIONS__ */
/*-----------------------------------------------------------------------------------*/

