/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TCL_ArraySize.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
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
			Scierror(999,_("Tcl Error: %s\n"),Tcl_GetStringResult(TCLinterpreter));
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
/*--------------------------------------------------------------------------*/
