#include "gw_symbolic.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
/*-----------------------------------------------------------------------------------*/

extern int C2F(sci_addf) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_subf) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_mulf) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_ldivf) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_rdivf) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
static SymbolicTable Tab[]=
{
{C2F(sci_addf),"addf"},
{C2F(sci_subf),"subf"},
{C2F(sci_mulf),"mulf"},
{C2F(sci_ldivf),"ldivf"},
{C2F(sci_rdivf),"rdivf"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_symbolic)()
{  
	if (Rhs != 2)
	{
		Scierror(39,"incorrect number of arguments.\r\n");
		return 0;
	}
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
