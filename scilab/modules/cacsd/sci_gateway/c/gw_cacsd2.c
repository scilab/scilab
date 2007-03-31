/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_cacsd2.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/

typedef int (*Cacsd2_Interf) __PARAMS((char *fname,unsigned long l));
typedef struct table_struct 
{
	Cacsd2_Interf f;    /** function **/
	char *name;      /** its name **/
} Cacsd2Table;

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
