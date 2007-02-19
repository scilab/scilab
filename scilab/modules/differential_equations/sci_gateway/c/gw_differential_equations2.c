/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/

#include "gw_differential_equations2.h"

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
#if _MSC_VER
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
