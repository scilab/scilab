/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_differential_equations3.h"
/*-----------------------------------------------------------------------------------*/
static int C2F(sci_odc) _PARAMS((char *fname,unsigned long l))
{
	extern int C2F(sciodc)();
	C2F(sciodc)();
	return 0;
}
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
