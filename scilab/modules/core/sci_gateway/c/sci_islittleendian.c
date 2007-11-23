/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "gw_core.h"
#include "stack-c.h"
#include "islittleendian.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_islittleendian) _PARAMS((char *fname,unsigned long fname_len))
{
	static int n1,m1;
	int *Status=NULL;

	CheckRhs(0,0);
	CheckLhs(1,1);

	Status=(int*)MALLOC(sizeof(int));
	*Status = islittleendian();

	m1=1;n1=1;
	CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &Status);
	LhsVar(1)=Rhs+1;

	C2F(putlhsvar)();

	if (Status) {FREE(Status);Status=NULL;}

	return 0;
}
/*--------------------------------------------------------------------------*/
