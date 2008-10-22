/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
 * @TODO: add comment
 *
 * @param TCLinterpreter    
 * @param VarName   
 * @param Str   
 * @param m 
 * @param n 
 * @return <ReturnValue>
 */
int SetVarStrings(Tcl_Interp *TCLinterpreter,char *VarName,char **Str,int m,int n);

/**
 * @TODO: add comment
 *
 * @param TCLinterpreter    
 * @param VarName   
 * @param Str   
 * @return <ReturnValue>
 */
int SetVarAString(Tcl_Interp *TCLinterpreter,char *VarName,char **Str);

/**
 * @TODO: add comment
 *
 * @param TCLinterpreter    
 * @param VarName   
 * @param ptrValues 
 * @param m 
 * @param n 
 * @return <ReturnValue>
 */
int SetVarMatrix(Tcl_Interp *TCLinterpreter,char *VarName,int ptrValues,int m,int n);

/**
 * @TODO: add comment
 *
 * @param TCLinterpreter    
 * @param VarName   
 * @param VarValue  
 * @return <ReturnValue>
 */
int SetVarScalar(Tcl_Interp *TCLinterpreter,char *VarName,double VarValue);
