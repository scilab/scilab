/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "history.h"
#include "gw_core.h"
#include "gw_shell.h"
#include "machine.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_resethistory) _PARAMS((char *fname,unsigned long fname_len))
{
	char Commentline[MAXBUF];	

	CheckRhs(0,0) ;
	CheckLhs(0,1) ;

	reset_history();

	GetCommentDateSession(Commentline,TRUE);		
	AddHistory (Commentline);

	LhsVar(1)=0;
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
