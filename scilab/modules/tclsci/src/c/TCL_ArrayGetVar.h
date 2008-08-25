/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TCL_ARRAYGETVAR_H__
#define __TCL_ARRAYGETVAR_H__

#include "TCL_Global.h"

/**
* TCL_ArrayGetVar
* @param[in] Tcl_Interp : TCL interpreter
* @param[in] VarName : Variable Name (Tcl Array)
* @param[in] index : index in TCL VarName Hashtabl
* @return value (string)
*/
char *TCL_ArrayGetVar(Tcl_Interp *TCLinterpreter,char *VarName,char *index);

#endif /* __TCL_ARRAYGETVAR_H__ */
/*--------------------------------------------------------------------------*/
