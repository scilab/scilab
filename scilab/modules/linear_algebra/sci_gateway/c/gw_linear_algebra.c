#include "gw_linear_algebra.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#if _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
/*-----------------------------------------------------------------------------------*/ 
extern int C2F(inthess) _PARAMS((char *fname, unsigned long fname_len));
extern int C2F(intschur) _PARAMS((char *fname, unsigned long fname_len));
extern int C2F(intbdiagr)  _PARAMS((char *fname, unsigned long fname_len));
extern int C2F(inteig) _PARAMS((char *fname, unsigned long fname_len));
extern int C2F(intbdiagr) _PARAMS((char *fname, unsigned long fname_len));
extern int C2F(intbalanc) _PARAMS((char *fname, unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/ 
static int C2F(intvoid) _PARAMS((char *fname, unsigned long fname_len))
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
static LinearAlgebraTable Tab[]={
  {C2F(inthess),"hess"},
  {C2F(intschur),"schur"},
  {C2F(inteig),"spec"},
  {C2F(intbdiagr),"bdiag"},
  {C2F(intvoid),"xxxx"},
  {C2F(intbalanc),"balanc"}
};
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_linear_algebra)()
{  
	Rhs = Max(0, Rhs);
	#if _MSC_VER
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

	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
