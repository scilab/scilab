/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclGet.h"
/*-----------------------------------------------------------------------------------*/
extern void flushTKEvents(void);
/*-----------------------------------------------------------------------------------*/
int C2F(intTclGet) _PARAMS((char *fname))
{
	CheckLhs(1,1);
	CheckRhs(2,2);

	if ( IsAScalar(Rhs-1) && (GetType(Rhs)==sci_strings) )
	{
		static int l1,n1,m1;
		int Handle=0;

		GetRhsVar(Rhs-1,"i",&m1,&n1,&l1);
		Handle=*istk(l1);

		TCL_UiGet(Handle,Rhs);
		flushTKEvents ();
	}
	else
	{
		Scierror(999,TCL_ERROR8);
		return 0;
	}

	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();	

	return 0;
}
/*-----------------------------------------------------------------------------------*/
