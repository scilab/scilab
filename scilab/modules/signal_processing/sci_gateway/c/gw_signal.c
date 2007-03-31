/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_signal.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*Signal_Interf) __PARAMS((char *fname,unsigned long l));
typedef struct table_struct 
{
	Signal_Interf f;    /** function **/
	char *name;      /** its name **/
} SignalTable;
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
#ifdef _MSC_VER
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
