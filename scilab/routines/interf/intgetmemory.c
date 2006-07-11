#include "../stack-c.h"
#include "intgetmemory.h"
#if WIN32|| defined (linux)
#include <string.h>
#endif

/*-----------------------------------------------------------------------------------
* INRIA 2005 
* Allan CORNET 
*-----------------------------------------------------------------------------------*/
int C2F(intgetmemory) _PARAMS((char *fname))
{
	static int l1,one=1;

	Rhs=Max(Rhs,0);
	CheckRhs(0,0) ;
	CheckLhs(1,2);

	CreateVar(1,"i",&one,&one,&l1);
	*istk(l1) = getfreememory();
	LhsVar(1) = 1;
	if (Lhs == 2) {
	  CreateVar(2,"i",&one,&one,&l1);
	  *istk(l1) = getmemorysize();
	  LhsVar(2) = 2;
	}

	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
