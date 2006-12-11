#include "gw_cacsd0.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
/*-----------------------------------------------------------------------------------*/
extern int C2F(sci_gschur) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_gspec) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_ereduc) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_fstair) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
int C2F(gw_cacsd0)(void);
/*-----------------------------------------------------------------------------------*/
static Cacsd0Table Tab[]=
{
	{C2F(sci_gschur),"gschur"},
	{C2F(sci_gspec),"gspec"},
	{C2F(sci_ereduc),"ereduc"},
	{C2F(sci_fstair),"fstair"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_cacsd0)(void)
{  
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
