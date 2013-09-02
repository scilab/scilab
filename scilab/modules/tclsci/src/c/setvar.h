/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __SETVAR_H__
#define __SETVAR_H__

#include "TCL_Global.h"
#include "BOOL.h"
/**
* Set variable "strings array" in TCL
* @param TCLinterpreter TCL interpreter
* @param VarName   TCL Variable name
* @param Str   values (string)
* @param m  row
* @param n  col
* @return TRUE or FALSE
*/
BOOL SetVarStrings(Tcl_Interp *TCLinterpreter, char *VarName, char **Str, int m, int n);

/**
* Set a variable in TCL
* @param TCLinterpreter TCL interpreter
* @param VarName  TCL variable name
* @param Str  value
* @return TRUE or FALSE
*/
BOOL SetVarAString(Tcl_Interp *TCLinterpreter, char *VarName, char **Str);

/**
* Set a "matrix" in TCL
* @param TCLinterpreter  TCL interpreter
* @param VarName TCL variable name
* @param ptrValues Scilab ptr on stack stk(ptrValues)
* @param m row
* @param n col
* @return TRUE or FALSE
*/
BOOL SetVarMatrix(Tcl_Interp *TCLinterpreter, char *VarName, int ptrValues, int m, int n);

/**
* Set a scalar in TCL
* @param TCLinterpreter   TCL interpreter
* @param VarName  TCL variable name
* @param VarValue value
* @return TRUE or FALSE
*/
BOOL SetVarScalar(Tcl_Interp *TCLinterpreter, char *VarName, double VarValue);

#endif /* __SETVAR_H__ */
/*--------------------------------------------------------------------------*/
