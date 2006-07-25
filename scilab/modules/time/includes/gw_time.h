/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_TIME__
#define __GW_TIME__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include "machine.h"
#include "stack-c.h"

typedef int (*time_interf) __PARAMS((char *fname,unsigned long fname_len));

typedef struct table_struct {
  time_interf f;    /** function **/
  char *name;      /** its name **/
} intTimeTable;

#endif /*  __GW_TIME__ */
/*-----------------------------------------------------------------------------------*/
