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
	static int n1 = 0,m1 = 0,l1 = 0;
	
	CheckRhs(0,0);
	CheckLhs(0,1);

	m1= (int)strlen(SCILABDEFAULTLANGUAGE);
	n1=1;

	CreateVar( Rhs+1,STRING_DATATYPE,&m1,&n1,&l1);
	strcpy(cstk(l1), SCILABDEFAULTLANGUAGE );

	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
