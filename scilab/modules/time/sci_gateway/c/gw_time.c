/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
	#include <windows.h>
	#include "ExceptionMessage.h"
#endif
#include "gw_time.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*time_interf) __PARAMS((char *fname,unsigned long fname_len));
typedef struct table_struct {
	time_interf f;    /** function **/
	char *name;      /** its name **/
} intTimeTable;
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
