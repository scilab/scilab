#include "gw_signal.h"
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
static SignalTable Tab[]=
{
	{C2F(sci_ffir),"ffir"},
	{C2F(sci_fft),"fft"},
	{C2F(sci_fiir),"fiir"},
	{C2F(sci_corr),"corr"},
	{C2F(sci_rpem),"rpem"},
	{C2F(sci_amell),"amell"},
	{C2F(sci_delip),"delip"},
	{C2F(sci_remez),"remez"},
	{C2F(sci_syredi),"syredi"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_signal)()
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
