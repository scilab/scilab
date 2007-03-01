#include "gw_linear_algebra2.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_linear_algebra.h"
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_linear_algebra2)(void);
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
int C2F(gw_linear_algebra2)(void)
{  
	Rhs = Max(0, Rhs);
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

	if (Err <= 0 && C2F(errgst).err1 <= 0) C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
