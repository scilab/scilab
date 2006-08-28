/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include "gw_special_functions1.h"
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
	#include "MALLOC.h"
	extern char *GetExceptionString(DWORD ExceptionCode);
#endif
/*-----------------------------------------------------------------------------------*/
extern int C2F(sci_oldbesseli) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_oldbesselj) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_oldbesselk) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_oldbessely) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_gamma) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_lgamma) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_dlgamma) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_calerf) _PARAMS((char *fname,unsigned long fname_len));
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
	return 0;
}
/*-----------------------------------------------------------------------------------*/
