#include "gw_optim.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_optim.h"
/*-----------------------------------------------------------------------------------*/
static OptimTable Tab[]=
{
{C2F(sci_optim),"optim"},
#ifdef NON_FREE_SCILAB
{C2F(sciquapro),"qpqpqp"}, /* primitive non documentee utilisee par quapro macro*/
#endif
{C2F(sci_semidef),"semidef"},
{C2F(sci_fsolve),"fsolve"},
{C2F(sci_sqrsolve),"sqrsolve"},
{C2F(sci_qld),"qld"},
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_optim)()
{  
	Rhs = Max(0, Rhs);
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
