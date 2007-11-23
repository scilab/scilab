/*--------------------------------------------------------------------------*/
/**
* @author Allan CORNET INRIA 2007
*/
/*--------------------------------------------------------------------------*/
#include "MALLOC.h"
#include "machine.h"
#include "stack-c.h"
#include "getlookandfeel.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_getlookandfeel) _PARAMS((char *fname,unsigned long fname_len))
{
	static int n1 = 0,m1 = 0;
	char *look= NULL;

	Rhs = Max(0, Rhs);
	CheckRhs(0,0);
	CheckLhs(1,1);

	look = getlookandfeel();

	if (look)
	{
		m1=(int)strlen(look);
		n1=1;
		CreateVarFromPtr(Rhs+1,STRING_DATATYPE,&m1,&n1,&look);
		if (look) {FREE(look); look = NULL;}
	}
	else
	{
		int l1=0;
		m1=0;
		n1=0;
		CreateVar(Rhs+1,STRING_DATATYPE,  &m1, &n1, &l1);
	}
	LhsVar(1) = Rhs+1;
	
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
