/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2008-2088 - INRIA - Bruno JOFRET
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
/*
** TCL Arrays are Hashtable in fact.
** So we have to access it through string indexes.
*/
char *TCL_ArrayGetVar(Tcl_Interp *TCLinterpreter,char *VarName,char *index)
{
	char *RetStr=NULL;
	char ArrayName[2048];

	if (index == NULL)
	  {
	    return strdup(TCL_NOT_DEFINE);
	  }

	sprintf(ArrayName,"%s(%s)",VarName,index);

	RetStr = (char*)Tcl_GetVar(TCLinterpreter, ArrayName, TCL_GLOBAL_ONLY);

	if (RetStr)
	{
		return strdup(RetStr);
	}
	else
	{
		return strdup(TCL_NOT_DEFINE);
	}
}
/*--------------------------------------------------------------------------*/
