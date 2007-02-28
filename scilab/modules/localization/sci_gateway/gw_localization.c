#include "gw_localization.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/
static LocalizationTable Tab[]=
{
	{C2F(sci_setlanguage),"setlanguage"},
	{C2F(sci_getlanguage),"getlanguage"},
	{C2F(sci_getlanguagealias),"getlanguagealias"},
	{C2F(sci_getlanguagestable),"getlanguagestable"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_localization)()
{  
	Rhs = Max(0, Rhs);
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
