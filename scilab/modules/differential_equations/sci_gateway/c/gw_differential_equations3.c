/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <windows.h>
#include <stdio.h>
#include "ExceptionMessage.h"
#endif
#include "gw_differential_equations3.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*Differential_Equations3_Interf) __PARAMS((char *fname,unsigned long l));
typedef struct table_struct 
{
	Differential_Equations3_Interf f;    /** function **/
	char *name;      /** its name **/
} DifferentialEquations3Table;
/*-----------------------------------------------------------------------------------*/
static DifferentialEquations3Table Tab[]=
{
	{C2F(sci_odc),"odc"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_differential_equations3)(void)
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
