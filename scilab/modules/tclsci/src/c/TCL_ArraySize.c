/*-----------------------------------------------------------------------------------*/
/* CORNET Allan */
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "TCL_ArraySize.h"
#include "Scierror.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/
int TCL_ArraySize(Tcl_Interp *TCLinterpreter,char *VarName)
{
	int ArraySize=0;

	if (strcmp(VarName,"TclScilabTmpVar"))
	{
		char MyTclCommand[2048];
		char *StrArraySize=NULL;

		sprintf(MyTclCommand, "set TclScilabTmpVar [array size %s];",VarName); 

		if ( Tcl_Eval(TCLinterpreter,MyTclCommand) == TCL_ERROR  )
		{
			Scierror(999,_("Tcl Error : %s\n"),TCLinterpreter->result);
			return 0;
		}

		StrArraySize = (char *) Tcl_GetVar(TCLinterpreter, "TclScilabTmpVar",TCL_GLOBAL_ONLY);

		if (StrArraySize)
		{
			ArraySize=(int)atoi(StrArraySize);
			Tcl_UnsetVar(TCLinterpreter, "TclScilabTmpVar", TCL_GLOBAL_ONLY);
		}
	}
	return ArraySize;
}
/*-----------------------------------------------------------------------------------*/
