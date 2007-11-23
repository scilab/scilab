/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "callFunctionFromGateway.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
void callFunctionFromGateway(gw_generic_table *Tab)
{
#ifdef _MSC_VER
#ifndef _DEBUG
	_try
	{
		if (*(Tab[Fin-1].f) != NULL) (*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
	}
	_except (EXCEPTION_EXECUTE_HANDLER)
	{	
		ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
	}
#else
	if (*(Tab[Fin-1].f) != NULL) (*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
#endif
#else
	if (*(Tab[Fin-1].f) != NULL) (*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
#endif
}
/*--------------------------------------------------------------------------*/
