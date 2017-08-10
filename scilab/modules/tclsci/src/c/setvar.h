/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
BOOL SetVarMatrix(Tcl_Interp *TCLinterpreter, char *VarName, double* MatrixDouble, int m, int n);

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
