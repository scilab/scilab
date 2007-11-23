/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "gw_io.h"
#include "stack-c.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_getio) _PARAMS((char *fname,unsigned long fname_len))
{
	int one = 1;
	int four = 4;
	int *values = NULL;

	CheckRhs(0,0);
	CheckLhs(0,1);

	values = (int*)MALLOC(sizeof(int)*(one*four));

	values[0] =  C2F(iop).rio;
	values[1] =  C2F(iop).rte;
	values[2] =  C2F(iop).wio;
	values[3] =  C2F(iop).wte;

	CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE,&one,&four,&values);
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();

	if (values) {FREE(values);values = NULL;}

	return 0;
}
/*--------------------------------------------------------------------------*/
