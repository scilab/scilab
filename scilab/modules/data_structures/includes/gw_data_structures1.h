/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_DATA_STRUCTURES1__
#define __GW_DATA_STRUCTURES1__

#ifdef _MSC_VER
#include <windows.h>
#include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"


typedef int (*Data_structures1_Interf) __PARAMS((char *fname));

typedef struct table_struct 
{
	Data_structures1_Interf f;    /** function **/
	char *name;      /** its name **/
} DataStructures1Table;

#endif /*  __GW_DATA_STRUCTURES1__ */
/*-----------------------------------------------------------------------------------*/

