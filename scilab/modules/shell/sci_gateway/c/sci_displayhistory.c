/*-----------------------------------------------------------------------------------*/
/**
 * @author Allan CORNET INRIA 2007
 */
/*-----------------------------------------------------------------------------------*/
#include "gw_shell.h"
#include "MALLOC.h"
#include "machine.h"
#include "stack-c.h"
#include "HistoryManager_c.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_displayhistory) _PARAMS((char *fname,unsigned long fname_len))
{
	displayScilabHistory();
	LhsVar(1) = 0;
	C2F(putlhsvar)();	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
