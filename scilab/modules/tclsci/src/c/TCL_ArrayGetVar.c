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

#include <string.h>
#include <stdio.h>
#include "TCL_ArrayGetVar.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
#define TCL_NOT_DEFINE "#NOT DEF.#"
/*--------------------------------------------------------------------------*/
char *TCL_ArrayGetVar(Tcl_Interp *TCLinterpreter,char *VarName,int i,int j)
{
	char *RetStr=NULL;
	char * StrValue=NULL;
	char ArrayName[2048];

	sprintf(ArrayName,"%s(%d,%d)",VarName,i,j);

	RetStr = (char*)Tcl_GetVar(TCLinterpreter, ArrayName, TCL_GLOBAL_ONLY);

	if (RetStr)
	{
		StrValue = strdup(RetStr);
	}
	else
	{
		StrValue = strdup(TCL_NOT_DEFINE);
	}

	return StrValue;
}
/*--------------------------------------------------------------------------*/
