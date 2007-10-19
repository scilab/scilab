/*-----------------------------------------------------------------------------------*/
/**
 * @author Allan CORNET INRIA 2007
 */
/*-----------------------------------------------------------------------------------*/
#include "gw_history_manager.h"
#include "MALLOC.h"
#include "machine.h"
#include "stack-c.h"
#include "HistoryManager.h"
#include "Scierror.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_saveconsecutivecommands) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckRhs(0,1) ;
	CheckLhs(0,1) ;

	if (Rhs == 0)
	{
		int n1 = 1, l1 = 0;

		if ( getSaveConsecutiveDuplicateLinesInScilabHistory() )
		{
			CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
			*istk(l1)=(int)(TRUE);
		}
		else
		{
			CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
			*istk(l1)=(int)(FALSE);
		}
		LhsVar(1)=Rhs+1;
	}
	else
	{
		if (GetType(1) == sci_boolean)
		{
			int n1 = 0, l1 = 0, m1 = 0;
			BOOL bOK = FALSE;
			GetRhsVar(1,MATRIX_OF_BOOLEAN_DATATYPE,&m1,&n1,&l1);
			bOK = *istk(l1);	
			setSaveConsecutiveDuplicateLinesInScilabHistory(bOK);
			LhsVar(1) = 0;
		}
		else
		{
			Scierror(999,_("parameter must be a boolean.\n"));
			return 0;
		}
	}

	C2F(putlhsvar)();	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
