/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/* @author Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#include "sci_getlanguage.h"
#include "defaultlanguage.h"
#include "Scierror.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_getdefaultlanguage) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;
	char *Output=NULL;

	CheckRhs(0,0);
	CheckLhs(0,1);

	Output=SCILABDEFAULTLANGUAGE;

	n1=1;
	CreateVarFromPtr( Rhs+1,STRING_DATATYPE,(m1=(int)strlen(Output), &m1),&n1,&Output);
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
