/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "../../src/c/history.h"
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
extern int C2F(resethistory) _PARAMS((char *fname));
/*-----------------------------------------------------------------------------------*/
int C2F(sci_resethistory) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(resethistory)(fname);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(resethistory) _PARAMS((char *fname))
{
	char Commentline[MAXBUF];	

	Rhs=Max(Rhs,0);
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
