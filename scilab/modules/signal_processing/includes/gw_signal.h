/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_SIGNAL__
#define __GW_SIGNAL__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

typedef int (*Signal_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Signal_Interf f;    /** function **/
  char *name;      /** its name **/
} SignalTable;

#endif /*  __GW_SIGNAL__ */
/*-----------------------------------------------------------------------------------*/

