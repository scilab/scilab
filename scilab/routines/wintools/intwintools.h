#ifndef __intwintools__
#define  __intwintools__

#ifdef WIN32
#include <windows.h>
#include <stdio.h>
#endif

#include "../machine.h"
#include "../stack-c.h"

typedef int (*Wintools_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct {
  Wintools_Interf f;    /** function **/
  char *name;      /** its name **/
} WintoolsTable;


#endif /*  __intwintools__ */