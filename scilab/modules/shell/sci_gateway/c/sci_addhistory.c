/*-----------------------------------------------------------------------------------*/
/**
 * @author Allan CORNET INRIA 2007
 */
/*-----------------------------------------------------------------------------------*/
#include "gw_shell.h"
#include "MALLOC.h"
#include "machine.h"
#include "stack-c.h"
#include "HistoryManager.h"
#include "Scierror.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_addhistory) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;

	CheckRhs(1,1);
	CheckLhs(0,1);

	if (GetType(1) == sci_strings)
	{
		char **lines=NULL;
		BOOL bOK = FALSE;

		GetRhsVar(1,"S",&m1,&n1,&lines);
		bOK = appendLinesToScilabHistory(lines,m1*n1);

		LhsVar(1) = 0;
		C2F(putlhsvar)();	

	}
	else
	{
		Scierror(999,"parameter must be a string.\n");
		return 0;
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/
