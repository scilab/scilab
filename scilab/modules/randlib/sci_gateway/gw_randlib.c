/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif

#include "stack-c.h"
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/
extern int RandI(char *fname,unsigned long fname_len);
/*-----------------------------------------------------------------------------------*/
static TabF Tab[]={ 
	{RandI, "Rand"},
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_randlib)(void)
{
	Rhs = Max(0, Rhs);
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
