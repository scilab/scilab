/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_SPARSE__
#define __GW_SPARSE__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "../machine.h"
#include "../stack-c.h"


typedef int (*Sparse_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Sparse_Interf f;    /** function **/
  char *name;      /** its name **/
} SparseTable;

#endif /*  __GW_SPARSE__ */
/*-----------------------------------------------------------------------------------*/

