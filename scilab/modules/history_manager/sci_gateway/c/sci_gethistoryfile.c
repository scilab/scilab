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
int C2F(sci_gethistoryfile) _PARAMS((char *fname,unsigned long fname_len))
{
	char *filename = NULL;
	int m1 = 0, n1 = 0;

	CheckRhs(0,0) ;
	CheckLhs(0,1) ;

	filename = getFilenameScilabHistory();

	if (filename)
	{
		n1=1;
		CreateVarFromPtr(Rhs+ 1,STRING_DATATYPE,(m1=(int)strlen(filename), &m1),&n1,&filename);
		if (filename) {FREE(filename);filename=NULL;}
		LhsVar(1) = Rhs+1;
		C2F(putlhsvar)();	
	}
	else
	{
		Scierror(999,"Error Impossible to know filename.\n");
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
