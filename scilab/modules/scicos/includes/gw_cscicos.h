/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_CSCICOS__
#define __GW_CSCICOS__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

typedef int (*scicosc_interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct {
  scicosc_interf f;    /** function **/
  char *name;      /** its name **/
} intcscicosTable;

#endif /*  __GW_CSCICOS__ */
/*-----------------------------------------------------------------------------------*/
