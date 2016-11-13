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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TCL_ArraySize.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
int TCL_ArraySize(Tcl_Interp *TCLinterpreter, char *VarName)
{
    int ArraySize = 0;

    if (strcmp(VarName, "TclScilabTmpVar"))
    {
        char MyTclCommand[2048];
        char *StrArraySize = NULL;

        sprintf(MyTclCommand, "set TclScilabTmpVar [array size %s];", VarName);

        if ( Tcl_Eval(TCLinterpreter, MyTclCommand) == TCL_ERROR  )
        {
            Scierror(999, _("Tcl Error: %s\n"), Tcl_GetStringResult(TCLinterpreter));
            return 0;
        }

        StrArraySize = (char *) Tcl_GetVar(TCLinterpreter, "TclScilabTmpVar", TCL_GLOBAL_ONLY);

        if (StrArraySize)
        {
            ArraySize = (int)atoi(StrArraySize);
            Tcl_UnsetVar(TCLinterpreter, "TclScilabTmpVar", TCL_GLOBAL_ONLY);
        }
    }
    return ArraySize;
}
/*--------------------------------------------------------------------------*/
