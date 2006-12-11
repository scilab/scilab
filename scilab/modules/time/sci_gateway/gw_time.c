#include "gw_time.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "sciprint.h"
#if _MSC_VER
#include "ExceptionMessage.h"
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
