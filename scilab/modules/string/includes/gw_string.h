/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_STRING__
#define __GW_STRING__

#ifdef _MSC_VER
#include <windows.h>
#include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"


typedef int (*String_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
	String_Interf f;    /** function **/
	char *name;      /** its name **/
} StringTable;

#endif /*  __GW_STRING__ */
/*-----------------------------------------------------------------------------------*/
