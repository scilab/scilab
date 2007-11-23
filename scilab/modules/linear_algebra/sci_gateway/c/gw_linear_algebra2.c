/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_linear_algebra.h"
#include "gw_linear_algebra2.h"
#include "callFunctionFromGateway.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/ 
static gw_generic_table Tab[]={
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
/*--------------------------------------------------------------------------*/ 
int C2F(gw_linear_algebra2)(void)
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	if (Err <= 0 && C2F(errgst).err1 <= 0) C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/ 
