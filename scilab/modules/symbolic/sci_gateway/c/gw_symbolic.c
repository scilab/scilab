/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_symbolic.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*Symbolic_Interf) __PARAMS((char *fname,unsigned long l));
typedef struct table_struct 
{
	Symbolic_Interf f;    /** function **/
	char *name;      /** its name **/
} SymbolicTable;
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
#ifdef _MSC_VER
	#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{	
			ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
		}
	#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
	#endif
#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
