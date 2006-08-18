/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_CACSD2__
#define __GW_CACSD2__

#ifdef _MSC_VER
  #include <windows.h>
  #include <stdio.h>
#endif

#include <string.h>

#include "../machine.h"
#include "../stack-c.h"


typedef int (*Cacsd2_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
  Cacsd2_Interf f;    /** function **/
  char *name;      /** its name **/
} Cacsd2Table;

#endif /*  __GW_GW_CACSD2__ */
/*-----------------------------------------------------------------------------------*/

