/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __MATSCICOS__
#define __MATSCICOS__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include "machine.h"
#include "stack-c.h"


typedef int (*scicosc_interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct {
  scicosc_interf f;    /** function **/
  char *name;      /** its name **/
} intcscicosTable;

#endif /*  __MATSCICOS__ */
/*-----------------------------------------------------------------------------------*/
