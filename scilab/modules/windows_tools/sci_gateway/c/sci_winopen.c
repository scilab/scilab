/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_windows_tools.h"
#include "stack-c.h"
#include "winopen.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_winopen) _PARAMS((char *fname,unsigned long l))
{
	int m1 = 0,n1 = 0,l1 = 0;
	BOOL bOK = FALSE;

	CheckRhs(1,1);

	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
	bOK = winopen(cstk(l1));

	if (bOK) LhsVar(1) = 0;
	else
	{
		Scierror(999,"Couldn't Open this document\n");
		return 0;
	}
	
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
