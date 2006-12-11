#include "gw_cacsd2.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
/*-----------------------------------------------------------------------------------*/
extern int C2F(sci_ppol) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_tzer) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_freq) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_ltitr) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_rtitr) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
int C2F(gw_cacsd2)(void);
/*-----------------------------------------------------------------------------------*/
static int C2F(sci_xxxx)_PARAMS((char *fname,unsigned long fname_len))
{
	/* not used */
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static Cacsd2Table Tab[]=
{
	{C2F(sci_xxxx),"xxxx"}, /* not used */
	{C2F(sci_ppol),"ppol"},
	{C2F(sci_tzer),"tzer"},
	{C2F(sci_freq),"freq"},
	{C2F(sci_ltitr),"ltitr"},
	{C2F(sci_rtitr),"rtitr"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_cacsd2)(void)
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
