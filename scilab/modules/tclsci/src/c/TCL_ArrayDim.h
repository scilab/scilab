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

#ifndef __TCL_ARRAYDIM_H__
#define __TCL_ARRAYDIM_H__

#include "TCL_Global.h"

/**
* TCL_ArrayDim get dimension of a TCL Array
* @param[in] TCLinterpreter : TCL Interpreter
* @param[in] VarName : TCL variable name
* @param[out] nb_lines lines found in TCL array
* @param[out] nb_columns is 1 if TCL Array is an Hastable
* @return index_size number of indexes
*/
char **TCL_ArrayDim(Tcl_Interp *TCLinterpreter,char *VarName,int *nb_lines, int* nb_columns);

#endif /* __TCL_ARRAYDIM_H__ */
/*--------------------------------------------------------------------------*/
