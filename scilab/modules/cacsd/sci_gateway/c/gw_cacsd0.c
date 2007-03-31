/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
/*-----------------------------------------------------------------------------------*/
#include "gw_cacsd0.h"
#include "stack-c.h"
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*Cacsd0_Interf) __PARAMS((char *fname,unsigned long l));
typedef struct table_struct 
{
	Cacsd0_Interf f;    /** function **/
	char *name;      /** its name **/
} Cacsd0Table;
/*-----------------------------------------------------------------------------------*/
static Cacsd0Table Tab[]=
{
	{C2F(sci_gschur),"gschur"},
	{C2F(sci_gspec),"gspec"},
	{C2F(sci_ereduc),"ereduc"},
	{C2F(sci_fstair),"fstair"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_cacsd0)(void)
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
