/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2005-2008 - INRIA - Allan CORNET
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "InitTclTk.h"
#include "Scierror.h"
#include "localization.h"
#include "InitializeTclTk.h"
#include "GlobalTclInterp.h"
/*--------------------------------------------------------------------------*/
int sci_TCL_DeleteInterp(char *fname, unsigned long l)
{
    CheckRhs(0, 1);
    CheckLhs(1, 1);

    if (Rhs == 1)
    {

        if (!existsGlobalInterp())
        {
            Scierror(999, _("%s: Error main TCL interpreter not initialized.\n"), fname);
            return 0;
        }

        if (GetType(1) == sci_strings)
        {
            static int l2, n2, m2;
            Tcl_Interp *TCLinterpreter = NULL;

            GetRhsVar(1, STRING_DATATYPE, &m2, &n2, &l2);
            TCLinterpreter = Tcl_GetSlave(getTclInterp(), cstk(l2));
            releaseTclInterp();
            if (TCLinterpreter == NULL)
            {
                Scierror(999, _("%s: No such slave interpreter.\n"), fname);
                return 0;
            }
            else
            {
                Tcl_DeleteInterp(TCLinterpreter);
                TCLinterpreter = NULL;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
            return 0;
        }
    }
    else /* Rhs == 0 */
    {
        releaseTclInterp();

        CloseTCLsci();
        InitializeTclTk();
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
