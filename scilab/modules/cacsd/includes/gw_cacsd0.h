/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_CACSD0__
#define __GW_CACSD0__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

typedef int (*Cacsd0_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Cacsd0_Interf f;    /** function **/
  char *name;      /** its name **/
} Cacsd0Table;

#endif /*  __GW_CACSD0__ */
/*-----------------------------------------------------------------------------------*/

