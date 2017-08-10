/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2008-2088 - INRIA - Bruno JOFRET
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

#include <string.h>
#include <stdio.h>
#include "TCL_ArrayGetVar.h"
#include "sci_malloc.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
#define TCL_NOT_DEFINE "#NOT DEF.#"
/*--------------------------------------------------------------------------*/
/*
** TCL Arrays are Hashtable in fact.
** So we have to access it through string indexes.
*/
char *TCL_ArrayGetVar(Tcl_Interp *TCLinterpreter, char *VarName, char *index)
{
    char *RetStr = NULL;
    char ArrayName[2048];

    if (index == NULL)
    {
        return os_strdup(TCL_NOT_DEFINE);
    }

    sprintf(ArrayName, "%s(%s)", VarName, index);

    RetStr = (char*)Tcl_GetVar(TCLinterpreter, ArrayName, TCL_GLOBAL_ONLY);

    if (RetStr)
    {
        return os_strdup(RetStr);
    }
    else
    {
        return os_strdup(TCL_NOT_DEFINE);
    }
}
/*--------------------------------------------------------------------------*/
