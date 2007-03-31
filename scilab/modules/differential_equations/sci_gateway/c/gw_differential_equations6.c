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
/*-----------------------------------------------------------------------------------*/
#include "gw_differential_equations6.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*Differential_Equations6_Interf) __PARAMS((char *fname,unsigned long l));
typedef struct table_struct 
{
	Differential_Equations6_Interf f;    /** function **/
	char *name;      /** its name **/
} DifferentialEquations6Table;
/*-----------------------------------------------------------------------------------*/
static DifferentialEquations6Table Tab[]=
{
	{C2F(scibvode),"bvode"}
};

/*-----------------------------------------------------------------------------------*/
int C2F(gw_differential_equations6)(void)
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
