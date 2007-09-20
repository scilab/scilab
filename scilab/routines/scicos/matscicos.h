/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __MATSCICOS__
#define __MATSCICOS__

#ifdef WIN32
  #include <windows.h>
  #include <stdio.h>
#endif
#include <string.h>
#include "../machine.h"
#include "../stack-c.h"


typedef int (*des_interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct {
  des_interf f;    /** function **/
  char *name;      /** its name **/
} intcscicosTable;

#endif /*  __MATSCICOS__ */
/*-----------------------------------------------------------------------------------*/
