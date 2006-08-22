/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_STATISTICS__
#define __GW_STATISTICS__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"


typedef int (*Statistics_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Statistics_Interf f;    /** function **/
  char *name;      /** its name **/
} StatisticsTable;

#endif /*  __GW_STATISTICS__ */
/*-----------------------------------------------------------------------------------*/

