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
extern int C2F(scilyap) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(scisylv) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intricc)  _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
int C2F(gw_cacsd1)(void);
/*-----------------------------------------------------------------------------------*/
static Cacsd1Table Tab[]=
{
	{C2F(scilyap),"lyap"},
	{C2F(scisylv),"sylv"},
	{C2F(intricc),"ricc"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_cacsd1)(void)
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
