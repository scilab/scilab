/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
	#include <windows.h>
	#include "ExceptionMessage.h"
#endif
#include "gw_special_functions1.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*Special_Functions_Interf) __PARAMS((char *fname,unsigned long l));
typedef struct table_struct 
{
	Special_Functions_Interf f;    /** function **/
	char *name;      /** its name **/
} SpecialFunctionsTable;
/*-----------------------------------------------------------------------------------*/
static SpecialFunctionsTable Tab[]={ 
  {C2F(sci_oldbesseli),"oldbesseli"},
  {C2F(sci_oldbesselj), "oldbesselj"},
  {C2F(sci_oldbesselk),"oldbesselk"},
  {C2F(sci_oldbessely),"oldbessely"},
  {C2F(sci_gamma),"gamma"},
  {C2F(sci_lgamma),"gammaln"},
  {C2F(sci_dlgamma),"dlgamma"},
  {C2F(sci_calerf),"calerf"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_special_functions1)(void)
{
	Rhs = Max(0, Rhs);

	#ifdef _MSC_VER
		#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f))(Tab[Fin-1].name,strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
			ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
		}
		#else
			(*(Tab[Fin-1].f))(Tab[Fin-1].name,strlen(Tab[Fin-1].name));
		#endif
	#else
		(*(Tab[Fin-1].f))(Tab[Fin-1].name,strlen(Tab[Fin-1].name));
	#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
