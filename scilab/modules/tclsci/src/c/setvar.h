/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifndef __SETVAR_H__
#define __SETVAR_H__

#include "TCL_Global.h"

/**
* Set variable "strings array" in TCL
* @param TCLinterpreter TCL interpreter   
* @param VarName   TCL Variable name
* @param Str   values (string)
* @param m  row
* @param n  col
* @return TRUE or FALSE
*/
BOOL SetVarStrings(Tcl_Interp *TCLinterpreter,char *VarName,char **Str,int m,int n);

/**
* Set a variable in TCL
* @param TCLinterpreter TCL interpreter
* @param VarName  TCL variable name
* @param Str  value 
* @return TRUE or FALSE
*/
BOOL SetVarAString(Tcl_Interp *TCLinterpreter,char *VarName,char **Str);

/**
* Set a "matrix" in TCL
* @param TCLinterpreter  TCL interpreter  
* @param VarName TCL variable name  
* @param ptrValues Scilab ptr on stack stk(ptrValues)
* @param m row
* @param n col
* @return TRUE or FALSE
*/
BOOL SetVarMatrix(Tcl_Interp *TCLinterpreter,char *VarName,int ptrValues,int m,int n);

/**
* Set a scalar in TCL
* @param TCLinterpreter   TCL interpreter    
* @param VarName  TCL variable name 
* @param VarValue value
* @return TRUE or FALSE
*/
BOOL SetVarScalar(Tcl_Interp *TCLinterpreter,char *VarName,double VarValue);

#endif /* __SETVAR_H__ */
/*-----------------------------------------------------------------------------------*/
