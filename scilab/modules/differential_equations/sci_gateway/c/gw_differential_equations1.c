/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_differential_equations1.h"

/*-----------------------------------------------------------------------------------*/
static int C2F(sci_none_empty) _PARAMS((char *fname,unsigned long fname_len))
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static DifferentialEquations1Table Tab[]=
{
	{C2F(sci_ode),"ode"},
	{C2F(sci_none_empty),"sci_none_empty"},
	{C2F(sci_intg),"intg"},
	{C2F(sci_feval),"feval"},
	{C2F(sci_bvode),"bvode"}
};

/*-----------------------------------------------------------------------------------*/
int C2F(gw_differential_equations1)(void)
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
