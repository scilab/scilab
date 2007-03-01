/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/

#include "gw_differential_equations5.h"

/*-----------------------------------------------------------------------------------*/
static int C2F(sci_feval) _PARAMS((char *fname,unsigned long fname_len))
{
	extern int C2F(feval)();
	C2F(feval)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static DifferentialEquations5Table Tab[]=
{
	{C2F(sci_feval),"feval"}
};

/*-----------------------------------------------------------------------------------*/
int C2F(gw_differential_equations5)(void)
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
