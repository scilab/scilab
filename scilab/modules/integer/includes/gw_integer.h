/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_INTEGER__
#define __GW_INTEGER__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"


typedef int (*Integer_Interf) __PARAMS((char *fname));

typedef struct table_struct 
{
  Integer_Interf f;    /** function **/
  char *name;      /** its name **/
} IntegerTable;

#endif /*  __GW_INTEGER__ */
/*-----------------------------------------------------------------------------------*/

