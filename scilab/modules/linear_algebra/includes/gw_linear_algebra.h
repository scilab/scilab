/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_LINEAR_ALGEBRA__
#define __GW_LINEAR_ALGEBRA__

#ifdef _MSC_VER
#include <windows.h>
#include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

typedef int (*Linear_Algebra_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
	Linear_Algebra_Interf f;    /** function **/
	char *name;      /** its name **/
} LinearAlgebraTable;

#endif /*  __GW_LINEAR_ALGEBRA__ */
/*-----------------------------------------------------------------------------------*/

