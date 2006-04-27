#include "matsound.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if WIN32
#include <Windows.h>
#include "../os_specific/win_mem_alloc.h"
extern char *GetExceptionString(DWORD ExceptionCode);
#endif
/*-----------------------------------------------------------------------------------*/
/* interface for the previous function Table */ 
/*-----------------------------------------------------------------------------------*/ 
extern int intBeep __PARAMS((char *fname,unsigned long fname_len));
extern int intPlaysound __PARAMS((char *fname,unsigned long fname_len));
extern int intsavewave __PARAMS((char *fname,unsigned long fname_len));
extern int intloadwave __PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/ 
static intSoundTable Tab[]={
	{ intsavewave, "savewave"},
	{ intloadwave, "loadwave"},
	{intPlaysound,"PlaySound"},
	{intBeep,"beep"}
};
/*-----------------------------------------------------------------------------------*/ 
int C2F(matsound)()
{  
	Rhs = Max(0, Rhs);
	#if WIN32
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
