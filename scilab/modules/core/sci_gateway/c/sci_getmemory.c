/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "getmemory.h"
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_getmemory) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,one=1;

	Rhs=Max(Rhs,0);
	CheckRhs(0,0) ;
	CheckLhs(1,2);

	CreateVar(Rhs+1,"i",&one,&one,&l1);
	*istk(l1) = getfreememory();
	LhsVar(1) = Rhs+1;
	if (Lhs == 2) {
		CreateVar(Rhs+2,"i",&one,&one,&l1);
		*istk(l1) = getmemorysize();
		LhsVar(2) = Rhs+2;
	}

	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
