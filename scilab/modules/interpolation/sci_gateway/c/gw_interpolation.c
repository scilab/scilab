/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_interpolation.h"
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include "ExceptionMessage.h"
#endif
/*-----------------------------------------------------------------------------------*/
extern int intsplin(char *fname,unsigned long fname_len); 
extern int intlsq_splin(char *fname,unsigned long fname_len);
extern int intinterp1(char *fname,unsigned long fname_len); 
extern int intlinear_interpn(char *fname,unsigned long fname_len);
extern int intsplin2d(char *fname,unsigned long fname_len); 
extern int intinterp2d(char *fname,unsigned long fname_len); 
extern int intcshep2d(char *fname,unsigned long fname_len); 
extern int inteval_cshep2d(char *fname,unsigned long fname_len);
extern int intsplin3d(char *fname,unsigned long fname_len); 
extern int intinterp3d(char *fname,unsigned long fname_len); 
extern int intbsplin3val(char *fname,unsigned long fname_len) ;
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
