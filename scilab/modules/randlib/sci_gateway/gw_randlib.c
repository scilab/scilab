/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "win_mem_alloc.h"
extern char *GetExceptionString(DWORD ExceptionCode);
#endif

#include "stack-c.h"
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