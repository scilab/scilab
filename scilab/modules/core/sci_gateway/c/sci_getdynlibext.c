/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "MALLOC.h"
#include "machine.h"
#include "stack-c.h"
#include "gw_core.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_getdynlibext) _PARAMS((char *fname,unsigned long fname_len))
{
	#define nbcharsext 8
	static int n1 = 0, m1 = 0;
	int outIndex = 0 ;

	CheckRhs(0,0);
	CheckLhs(1,1);
	
	m1= (int)strlen(SHARED_LIB_EXT);
	n1=1;

	CreateVar( Rhs+1,STRING_DATATYPE,&m1,&n1,&outIndex);
	strcpy(cstk(outIndex), SHARED_LIB_EXT );

	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/
