/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __MATTIME__
#define __MATTIME__

#ifdef WIN32
  #include <windows.h>
  #include <stdio.h>
#endif

#include "../machine.h"
#include "../stack-c.h"

typedef int (*time_interf) __PARAMS((char *fname,unsigned long fname_len));

typedef struct table_struct {
  time_interf f;    /** function **/
  char *name;      /** its name **/
} intTimeTable;

#endif /*  __MATTIME__ */
/*-----------------------------------------------------------------------------------*/
