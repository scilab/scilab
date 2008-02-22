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

#ifndef __TCL_ARRAYDIM_H__
#define __TCL_ARRAYDIM_H__

#include "TCL_Global.h"

/**
* TCL_ArrayDim get dimension of a TCL Array
* @param[in] TCLinterpreter : TCL Interpreter
* @param[in] VarName : TCL variable name
* @param[out] m size row 
* @param[out] n size col
* @return FALSE or TRUE (OK)
*/
BOOL TCL_ArrayDim(Tcl_Interp *TCLinterpreter,char *VarName,int *m,int *n);

#endif /* __TCL_ARRAYDIM_H__ */
/*--------------------------------------------------------------------------*/
