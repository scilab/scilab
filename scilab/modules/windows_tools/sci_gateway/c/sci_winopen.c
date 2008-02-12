/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "gw_windows_tools.h"
#include "stack-c.h"
#include "winopen.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_winopen) _PARAMS((char *fname,unsigned long l))
{
	int m1 = 0,n1 = 0,l1 = 0;

	CheckRhs(1,1);

	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);

	if (winopen(cstk(l1))) 
		{ 
			LhsVar(1) = 0;
		}
	else
		{
			Scierror(999,_("%s: Cannot open file %s.\n"),fname,cstk(l1));
			return 0;
		}
	
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
