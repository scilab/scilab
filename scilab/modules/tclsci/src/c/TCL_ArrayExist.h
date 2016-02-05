/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
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
