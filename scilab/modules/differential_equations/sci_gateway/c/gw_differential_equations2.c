/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <windows.h>
#include <stdio.h>
#include "ExceptionMessage.h"
#endif
#include <string.h>
/*-----------------------------------------------------------------------------------*/
#include "gw_differential_equations2.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*Differential_Equations2_Interf) __PARAMS((char *fname,unsigned long l));
typedef struct table_struct 
{
	Differential_Equations2_Interf f;    /** function **/
	char *name;      /** its name **/
} DifferentialEquations2Table;
/*-----------------------------------------------------------------------------------*/
static DifferentialEquations2Table Tab[]=
{
	{C2F(sciimpl),"impl"},
	{C2F(dassli),"dassl"},
	{C2F(dasrti),"dasrt"},
};

/*-----------------------------------------------------------------------------------*/
int C2F(gw_differential_equations2)(void)
{  
#ifdef _MSC_VER
#ifndef _DEBUG
	_try
	{
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
	}
	_except (EXCEPTION_EXECUTE_HANDLER)
	{	
		ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
	}
#else
	(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
#endif
#else
	(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
