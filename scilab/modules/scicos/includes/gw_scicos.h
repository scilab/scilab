/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_SCICOS__
#define __GW_SCICOS__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

typedef int (*scicos_interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct {
  scicos_interf f;    /** function **/
  char *name;      /** its name **/
} intscicosTable;

#endif /*  __GW_SCICOS__ */
/*-----------------------------------------------------------------------------------*/
