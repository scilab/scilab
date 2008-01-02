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
int C2F(sci_saveafterncommands) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckRhs(0,1) ;
	CheckLhs(0,1) ;

	if (Rhs == 0)
	{
		int n1 = 1, l1 = 0;
		int num = getAfterHowManyLinesScilabHistoryIsSaved();
		CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &n1,&n1,&l1);
		*stk(l1) = (double)num;
		LhsVar(1) = Rhs+1;
	}
	else
	{
		if (GetType(1) == sci_matrix)
		{
			int n1 = 0, l1 = 0, m1 = 0;
			int num = FALSE;
			char *filename = getFilenameScilabHistory();

			filename = getFilenameScilabHistory();
			if (filename == NULL) 
			{
				setDefaultFilenameScilabHistory();
				filename = getFilenameScilabHistory();
			}

			GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
			num = (int)*stk(l1);	
			setAfterHowManyLinesScilabHistoryIsSaved(num);
			LhsVar(1) = 0;
		}
		else
		{
			Scierror(999,_("%s: Wrong type for first input argument: Integer expected.\n"), fname);
			return 0;
		}
	}

	C2F(putlhsvar)();	
	return 0;
}
/*--------------------------------------------------------------------------*/
