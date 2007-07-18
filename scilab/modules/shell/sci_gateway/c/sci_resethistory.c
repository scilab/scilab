/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_core.h"
#include "gw_shell.h"
#include "machine.h"
#include "stack-c.h"
#include "HistoryManager.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_resethistory) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckRhs(0,0) ;
	CheckLhs(0,1) ;

	resetScilabHistory();

	LhsVar(1)=0;
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
