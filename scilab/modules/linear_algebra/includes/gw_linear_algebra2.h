/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_LINEAR_ALGEBRA2__
#define __GW_LINEAR_ALGEBRA2__

#ifdef _MSC_VER
#include <windows.h>
#include <stdio.h>
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

typedef int (*Linear_Algebra2_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct2
{
	Linear_Algebra2_Interf f;    /** function **/
	char *name;      /** its name **/
} LinearAlgebra2Table;

#endif /*  __GW_LINEAR_ALGEBRA2__ */
/*-----------------------------------------------------------------------------------*/

