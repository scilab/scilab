/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __TCL_ARRAYSIZE_H__
#define __TCL_ARRAYSIZE_H__

#include "TCL_Global.h"

/**
* TCL_ArraySize
* @param[in] Tcl_Interp TCL Interpreter
* @param[in] VarName TCL Variable name
* @return 0 or 1 (OK)
*/
int TCL_ArraySize(Tcl_Interp *TCLinterpreter, char *VarName);

#endif /* __TCL_ARRAYSIZE_H__ */
/*--------------------------------------------------------------------------*/
