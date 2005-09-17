/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclSet.h"
/*-----------------------------------------------------------------------------------*/
extern void flushTKEvents(void);
/*-----------------------------------------------------------------------------------*/
int C2F(intTclSet) _PARAMS((char *fname))
{
	CheckLhs(1,1);
	CheckRhs(3,3);

	if ( IsAScalar(1) )
	{
		static int l1,n1,m1;
		int Handle=0;

		GetRhsVar(1,"i",&m1,&n1,&l1);
		Handle=*istk(l1);
		
		InterfaceScilabToUiSet(Handle,Rhs-1,Rhs);
		flushTKEvents ();
	}
	else
	{
		if (GetType(Rhs-1)==sci_strings)
		{
			Scierror(999,TCL_ERROR21);
			return 0;
		}
		if ( IsAScalar(Rhs-2) )
		{
			Scierror(999,TCL_ERROR22);
			return 0;
		}
    }

	LhsVar(1) = 0;
	C2F(putlhsvar)();	

	return 0;
}
/*-----------------------------------------------------------------------------------*/
