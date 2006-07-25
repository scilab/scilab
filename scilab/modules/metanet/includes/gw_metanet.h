/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_METANET__
#define __GW_METANET__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"


typedef int (*Metanet_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Metanet_Interf f;    /** function **/
  char *name;      /** its name **/
} MetanetTable;

#endif /*  __GW_METANET__ */
/*-----------------------------------------------------------------------------------*/

