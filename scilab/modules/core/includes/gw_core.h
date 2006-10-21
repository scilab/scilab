/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_CORE__
#define __GW_CORE__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

typedef int (*Core_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Core_Interf f;    /** function **/
  char *name;      /** its name **/
} CoreTable;

#endif /*  __GW_CORE__ */
/*-----------------------------------------------------------------------------------*/

