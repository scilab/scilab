/*-----------------------------------------------------------------------------------*/
/* CORNET Allan */
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include "TCL_ArrayExist.h"
#include "error_scilab.h"
/*-----------------------------------------------------------------------------------*/
#define TCL_VAR_NAME_TMP "TclScilabTmpVar"
/*-----------------------------------------------------------------------------------*/
BOOL TCL_ArrayExist(Tcl_Interp *TCLinterpreter,char *VarName)
{
	BOOL bExist = FALSE;

	if (strcmp(VarName,TCL_VAR_NAME_TMP))
	{
		char MyTclCommand[2048];
		char *StrArrayExist=NULL;

		sprintf(MyTclCommand, "set TclScilabTmpVar [array exists %s];",VarName); 

		if ( Tcl_Eval(TCLinterpreter,MyTclCommand) == TCL_ERROR  )
		{
			error_scilab(999,"Tcl Error : %s",TCLinterpreter->result);
			return FALSE;
		}

		StrArrayExist = (char *) Tcl_GetVar(TCLinterpreter, TCL_VAR_NAME_TMP,TCL_GLOBAL_ONLY);

		if (StrArrayExist)
		{
			int r  = (int)atoi(StrArrayExist);
			if (r) bExist = TRUE;
			Tcl_UnsetVar(TCLinterpreter,TCL_VAR_NAME_TMP, TCL_GLOBAL_ONLY);
		}
	}

	return bExist;
}
/*-----------------------------------------------------------------------------------*/
