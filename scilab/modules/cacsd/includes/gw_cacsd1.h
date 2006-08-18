/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_CACSD1__
#define __GW_CACSD1__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"


typedef int (*Cacsd1_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Cacsd1_Interf f;    /** function **/
  char *name;      /** its name **/
} Cacsd1Table;

#endif /*  __GW_CACSD1__ */
/*-----------------------------------------------------------------------------------*/

