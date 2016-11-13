/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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

#include <stdlib.h>
#include "TCL_ArrayExist.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
#define TCL_VAR_NAME_TMP "TclScilabTmpVar"
/*--------------------------------------------------------------------------*/
BOOL TCL_ArrayExist(Tcl_Interp *TCLinterpreter, char *VarName)
{
    BOOL bExist = FALSE;

    if (strcmp(VarName, TCL_VAR_NAME_TMP))
    {
        char MyTclCommand[2048];
        char *StrArrayExist = NULL;

        sprintf(MyTclCommand, "set TclScilabTmpVar [array exists %s];", VarName);

        if ( Tcl_Eval(TCLinterpreter, MyTclCommand) == TCL_ERROR  )
        {
            Scierror(999, _("Tcl Error : %s\n"), Tcl_GetStringResult(TCLinterpreter));
            return FALSE;
        }

        StrArrayExist = (char *) Tcl_GetVar(TCLinterpreter, TCL_VAR_NAME_TMP, TCL_GLOBAL_ONLY);

        if (StrArrayExist)
        {
            int r  = (int)atoi(StrArrayExist);
            if (r)
            {
                bExist = TRUE;
            }
            Tcl_UnsetVar(TCLinterpreter, TCL_VAR_NAME_TMP, TCL_GLOBAL_ONLY);
        }
    }

    return bExist;
}
/*--------------------------------------------------------------------------*/
