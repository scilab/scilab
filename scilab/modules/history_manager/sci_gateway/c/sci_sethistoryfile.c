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
/*--------------------------------------------------------------------------*/
int C2F(sci_sethistoryfile) _PARAMS((char *fname,unsigned long fname_len))
{
	char  *filename = NULL;

	CheckRhs(0,1) ;
	CheckLhs(0,1) ;

	if (Rhs == 0)
	{
		setDefaultFilenameScilabHistory();
	}
	else
	{
		int l1 = 0, m1 = 0, n1 = 0;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		filename=cstk(l1);
		setFilenameScilabHistory(filename);
	}
	LhsVar(1) = 0;
	C2F(putlhsvar)();	

	return 0;
}
/*--------------------------------------------------------------------------*/
