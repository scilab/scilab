#include "gw_signal.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
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
