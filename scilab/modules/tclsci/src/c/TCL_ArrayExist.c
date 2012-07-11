/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdlib.h>
#include "TCL_ArrayExist.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
#define TCL_VAR_NAME_TMP "TclScilabTmpVar"
/*--------------------------------------------------------------------------*/
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
			Scierror(999,_("Tcl Error : %s\n"),Tcl_GetStringResult(TCLinterpreter));
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
/*--------------------------------------------------------------------------*/
