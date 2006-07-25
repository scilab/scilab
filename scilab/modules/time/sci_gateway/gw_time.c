#include "gw_time.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "MALLOC.h"
#if _MSC_VER
#include <Windows.h>
extern char *GetExceptionString(DWORD ExceptionCode);
#endif
/*-----------------------------------------------------------------------------------*/
/* interface for the previous function Table */ 
/*-----------------------------------------------------------------------------------*/ 
extern int C2F(sci_getdate) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_calendar) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_timer) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_sleep) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_xpause) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_realtimeinit) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_realtime) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/ 
static intTimeTable Tab[]={
    {C2F(sci_getdate),"getdate"},
	{C2F(sci_calendar),"Calendar"},
	{C2F(sci_timer),"timer"},
	{C2F(sci_sleep),"sleep"},
	{C2F(sci_xpause),"xpause"},
	{C2F(sci_realtimeinit),"realtimeinit"},
	{C2F(sci_realtime),"realtime"},
};
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_time)()
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
