/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __TCL_ARRAYEXIST_H__
#define __TCL_ARRAYEXIST_H__

#include "TCL_Global.h"
#include "BOOL.h"

/**
* TCL_ArrayExist
* check if "VarName" array exist
* @param[in] TCLinterpreter
* @param[in] VarName
* @return FALSE or TRUE (Exist)
*/
BOOL TCL_ArrayExist(Tcl_Interp *TCLinterpreter, char *VarName);

#endif /* __TCL_ARRAYEXIST_H__ */
/*--------------------------------------------------------------------------*/
