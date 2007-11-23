/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "TCL_Global.h"
#include "setenvtcl.h"
/*--------------------------------------------------------------------------*/ 
int setenvtcl(char *string,char *value)
{
	int bOK=FALSE;
	char MyTclCommand[2048];

	sprintf(MyTclCommand,"env(%s)",string);

	if (TCLinterp==NULL) return((int)FALSE);
	
	if ( !Tcl_SetVar(TCLinterp,MyTclCommand, value, TCL_GLOBAL_ONLY) )
	{
		bOK=(int)(FALSE);
	}
	else
	{
		bOK=(int)(TRUE);
	}

	return bOK;
}
/*--------------------------------------------------------------------------*/
