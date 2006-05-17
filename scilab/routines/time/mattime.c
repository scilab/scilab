#include "mattime.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "../os_specific/win_mem_alloc.h"
extern char *GetExceptionString(DWORD ExceptionCode);
#endif
/*-----------------------------------------------------------------------------------*/
/* interface for the previous function Table */ 
/*-----------------------------------------------------------------------------------*/ 
extern int C2F(intgetdate) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intcalendar) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(inttimer) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsleep) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intxpause) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intrealtimeinit) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intrealtime) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/ 
static intTimeTable Tab[]={
  {C2F(intgetdate),"getdate"},
	{C2F(intcalendar),"Calendar"},
	{C2F(inttimer),"timer"},
	{C2F(intsleep),"sleep"},
	{C2F(intxpause),"xpause"},
	{C2F(intrealtimeinit),"realtimeinit"},
	{C2F(intrealtime),"realtime"},
};
/*-----------------------------------------------------------------------------------*/ 
int C2F(mattime)()
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
