/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include <string.h> /*pour strcmp */
#include <math.h>
#include <setjmp.h>
#include "gw_special_functions2.h"
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
	#include <Windows.h>
	#include "ExceptionMessage.h"
#endif
#include "stack-c.h"
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/
extern jmp_buf slatec_jmp_env; 

/*-----------------------------------------------------------------------------------*/
static TabF Tab[]={ 
  {sci_legendre, "legendre"},
  {sci_beta, "beta"},
  {sci_besseli,"besseli"},
  {sci_besselj,"besselj"},
  {sci_besselk,"besselk"},
  {sci_bessely,"bessely"},
  {sci_besselh,"besselh"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_special_functions2)(void)
{
	Rhs = Max(0, Rhs);
	if (setjmp(slatec_jmp_env)) 
	{ 
		Scierror(999,"%s: arguments must be positive \r\n", Tab[Fin-1].name);
		return 0;
	}

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

	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
