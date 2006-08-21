/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_CACSD3__
#define __GW_CACSD3__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"


typedef int (*Cacsd3_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Cacsd3_Interf f;    /** function **/
  char *name;      /** its name **/
} Cacsd3Table;

#endif /*  __GW_CACSD3__ */
/*-----------------------------------------------------------------------------------*/

