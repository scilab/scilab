/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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
char **TCL_ArrayDim(Tcl_Interp *TCLinterpreter, char *VarName, int *nb_lines, int* nb_columns);

#endif /* __TCL_ARRAYDIM_H__ */
/*--------------------------------------------------------------------------*/
