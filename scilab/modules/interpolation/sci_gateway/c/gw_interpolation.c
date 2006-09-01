/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_interpolation.h"
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
extern char *GetExceptionString(DWORD ExceptionCode);
#endif
/*-----------------------------------------------------------------------------------*/
intsplin(char *fname,unsigned long fname_len); 
intlsq_splin(char *fname,unsigned long fname_len);
intinterp1(char *fname,unsigned long fname_len); 
intlinear_interpn(char *fname,unsigned long fname_len);
intsplin2d(char *fname,unsigned long fname_len); 
intinterp2d(char *fname,unsigned long fname_len); 
intcshep2d(char *fname,unsigned long fname_len); 
inteval_cshep2d(char *fname,unsigned long fname_len);
intsplin3d(char *fname,unsigned long fname_len); 
intinterp3d(char *fname,unsigned long fname_len); 
intbsplin3val(char *fname,unsigned long fname_len) ;
/*-----------------------------------------------------------------------------------*/
static TabF Tab[]={ 
  {intsplin,           "splin"},
  {intlsq_splin,       "lsq_splin"},
  {intinterp1,          "interp"},
  {intlinear_interpn,  "linear_interpn"},
  {intsplin2d,         "splin2d"},
  {intinterp2d,        "interp2d"},
  {intcshep2d,         "cshep2d"},
  {inteval_cshep2d,    "eval_cshep2d" },
  {intsplin3d,         "splin3d"},
  {intinterp3d,        "interp3d"},
  {intbsplin3val,      "bsplin3val"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_interpolation)(void)
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