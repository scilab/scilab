/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include <string.h> /*pour strcmp */
#include <math.h>
#include <setjmp.h>
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
	#include "../os_specific/win_mem_alloc.h"
	extern char *GetExceptionString(DWORD ExceptionCode);
#endif
#include "../stack-c.h"
/*-----------------------------------------------------------------------------------*/
extern jmp_buf slatec_jmp_env; 
extern int sci_besselh(char *fname,unsigned long fname_len);
extern int sci_besseli(char *fname,unsigned long fname_len);
extern int sci_besselj(char *fname,unsigned long fname_len);
extern int sci_besselk(char *fname,unsigned long fname_len);
extern int sci_bessely(char *fname,unsigned long fname_len);
extern int sci_beta(char *fname,unsigned long fname_len);
extern int sci_legendre(char *fname,unsigned long fname_len);
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
int C2F(otherspfunlib)(void)
{
	Rhs = Max(0, Rhs);
	if (setjmp(slatec_jmp_env)) 
	{ 
		Scierror(999,"%s: arguments must be positive \r\n", Tab[Fin-1].name);
		return 0;
	}

	#if _MSC_VER
		#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f))(Tab[Fin-1].name,strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
			char *ExceptionString=GetExceptionString(GetExceptionCode());
			sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nScilab may become unstable.\n",ExceptionString,Tab[Fin-1].name);
			if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
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
