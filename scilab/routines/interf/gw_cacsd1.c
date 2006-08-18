#include "gw_cacsd1.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "MALLOC.h"
extern char *GetExceptionString(DWORD ExceptionCode);
#endif
/*-----------------------------------------------------------------------------------*/
extern int C2F(sci_gschur) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_gspec) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_ereduc) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_fstair) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
static Cacsd1Table Tab[]=
{
	{C2F(sci_gschur),"gschur"},
	{C2F(sci_gspec),"gspec"},
	{C2F(sci_ereduc),"ereduc"},
	{C2F(sci_fstair),"fstair"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_cacsd1)()
{  
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
