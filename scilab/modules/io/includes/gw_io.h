/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_IO__
#define __GW_IO__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

typedef int (*Io_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Io_Interf f;    /** function **/
  char *name;      /** its name **/
} IoTable;

#endif /*  __GW_IO__ */
/*-----------------------------------------------------------------------------------*/

