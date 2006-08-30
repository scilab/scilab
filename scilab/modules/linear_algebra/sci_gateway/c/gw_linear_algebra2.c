#include "gw_linear_algebra2.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#if _MSC_VER
#include <Windows.h>
#include "MALLOC.h"
extern char *GetExceptionString(DWORD ExceptionCode);
#endif
/*-----------------------------------------------------------------------------------*/ 
extern int C2F(intqr) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intsvd) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intlsq) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(inteig) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intinv) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intrcond) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intchol) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intlu) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intslash) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intbackslash) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intschur) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(inthess) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intdet) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intbalanc) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/ 
static LinearAlgebra2Table Tab[]={
	{C2F(intqr),"qr"},
	{C2F(intsvd),"svd"},
	{C2F(intlsq),"lsq"},
	{C2F(inteig),"spec"},
	{C2F(intinv),"inv"},
	{C2F(intrcond),"rcond"},
	{C2F(intchol),"chol"},
	{C2F(intlu),"lu"},
	{C2F(intslash),"slash"},
	{C2F(intbackslash),"backslash"},
	{C2F(intschur),"schur"},
	{C2F(inthess),"hess"},
	{C2F(intdet),"det"},
	{C2F(intbalanc),"balanc"},
};
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_linear_algebra2)()
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
		char *ExceptionString=GetExceptionString(GetExceptionCode());
		sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nScilab may become unstable.\n",ExceptionString,Tab[Fin-1].name);
		if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
	}
#else
	(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
#endif
#else
	(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
#endif

	if (Err <= 0 && C2F(errgst).err1 <= 0) C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
