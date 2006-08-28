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
extern int intbesselh(char *fname,unsigned long fname_len);
extern int intbesseli(char *fname,unsigned long fname_len);
extern int intbesselj(char *fname,unsigned long fname_len);
extern int intbesselk(char *fname,unsigned long fname_len);
extern int intbessely(char *fname,unsigned long fname_len);
extern int intbeta(char *fname,unsigned long fname_len);
extern int intlegendre(char *fname,unsigned long fname_len);
/*-----------------------------------------------------------------------------------*/
static TabF Tab[]={ 
  {intlegendre, "legendre"},
  {intbeta, "beta"},
  {intbesseli,"besseli"},
  {intbesselj,"besselj"},
  {intbesselk,"besselk"},
  {intbessely,"bessely"},
  {intbesselh,"besselh"}
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
