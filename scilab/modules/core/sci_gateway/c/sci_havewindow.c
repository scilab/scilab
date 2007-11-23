/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "scilabmode.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_havewindow) _PARAMS((char *fname,unsigned long fname_len))
{
	static int n1,m1;
	int *Status=NULL;

	CheckRhs(0,1);
	CheckLhs(1,1);

	Status=(int*)MALLOC(sizeof(int));
	*Status = (int) ( getScilabMode() == SCILAB_STD );

	m1=1;n1=1;
	CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &Status);
	LhsVar(1)=Rhs+1;

	C2F(putlhsvar)();

	if (Status) {FREE(Status);Status=NULL;}

	return 0;
}
/*--------------------------------------------------------------------------*/
