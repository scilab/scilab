/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/* @author Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#include "gw_localization.h"
#include "defaultlanguage.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_getdefaultlanguage) _PARAMS((char *fname,unsigned long fname_len))
{
	static int n1,m1;
	char *Output=NULL;

	CheckRhs(0,0);
	CheckLhs(0,1);

	Output=SCILABDEFAULTLANGUAGE;
	strcat(Output,"\0");
	n1=1;
	CreateVarFromPtr( Rhs+1,STRING_DATATYPE,(m1=(int)strlen(Output), &m1),&n1,&Output);
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
