#include "gw_optim.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "MALLOC.h"
extern char *GetExceptionString(DWORD ExceptionCode);
#endif
/*-----------------------------------------------------------------------------------*/
extern int C2F(sci_optim) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sciquapro) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_semidef) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_fsolve) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_sqrsolve) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_qld) _PARAMS((char *fname,unsigned long fname_len));

/*-----------------------------------------------------------------------------------*/
static OptimTable Tab[]=
{
{C2F(sci_optim),"optim"},
{C2F(sciquapro),"qpqpqp"}, /* primitive non documentee utilisee par quapro macro*/
{C2F(sci_semidef),"semidef"},
{C2F(sci_fsolve),"fsolve"},
{C2F(sci_sqrsolve),"sqrsolve"},
{C2F(sci_qld),"qld"}
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
			char *ExceptionString=GetExceptionString(GetExceptionCode());
			sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nScilab may become unstable.\n",ExceptionString,Tab[Fin-1].name);
			if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
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
