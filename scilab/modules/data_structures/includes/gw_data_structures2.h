/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_DATA_STRUCTURES2__
#define __GW_DATA_STRUCTURES2__

#ifdef _MSC_VER
#include <windows.h>
#include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"


typedef int (*Data_structures2_Interf) __PARAMS((char *fname,unsigned long fname_len));

typedef struct table_struct 
{
	Data_structures2_Interf f;    /** function **/
	char *name;      /** its name **/
} DataStructures2Table;

#endif /*  __GW_DATA_STRUCTURES2__ */
/*-----------------------------------------------------------------------------------*/

