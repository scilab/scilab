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
/*-----------------------------------------------------------------------------------*/
int C2F(sci_displayhistory) _PARAMS((char *fname,unsigned long fname_len))
{
	displayScilabHistory();
	LhsVar(1) = 0;
	C2F(putlhsvar)();	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
