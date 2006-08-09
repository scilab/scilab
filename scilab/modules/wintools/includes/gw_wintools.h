/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __MATWINTOOLS__
#define __MATWINTOOLS__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include "machine.h"
#include "stack-c.h"

typedef int (*Wintools_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct {
  Wintools_Interf f;    /** function **/
  char *name;      /** its name **/
} WintoolsTable;


#endif /*  __MATWINTOOLS__ */
/*-----------------------------------------------------------------------------------*/

