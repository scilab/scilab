/*--------------------------------------------------------------------------*/
/**
 * @author Allan CORNET INRIA 2007
 */
/*--------------------------------------------------------------------------*/
#include "gw_history_manager.h"
#include "MALLOC.h"
#include "machine.h"
#include "stack-c.h"
#include "HistoryManager.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_addhistory) _PARAMS((char *fname,unsigned long fname_len))
{
	static int n1,m1;

	CheckRhs(1,1);
	CheckLhs(0,1);

	if (GetType(1) == sci_strings)
	{
		char **lines=NULL;
		BOOL bOK = FALSE;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&lines);
		bOK = appendLinesToScilabHistory(lines,m1*n1);

		LhsVar(1) = 0;
		C2F(putlhsvar)();	

	}
	else
	{
		Scierror(999,_("Invalid type of input argument: String or matrix of string expected.\n"));
		return 0;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
