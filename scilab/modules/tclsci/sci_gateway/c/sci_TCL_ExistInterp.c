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
#include "Scierror.h"
#include "localization.h"
#include "GlobalTclInterp.h"
/*--------------------------------------------------------------------------*/
int sci_TCL_ExistInterp(char *fname, unsigned long l)
{
    static int l1 = 0, n1 = 0, m1 = 0;

    char *InterpName = NULL;

    CheckRhs(1, 2);
    CheckLhs(1, 1);

    if (GetType(1) == sci_strings)
    {
        GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);

        if (!existsGlobalInterp())
        {
            Scierror(999, _("%s: Error main TCL interpreter not initialized.\n"), fname);
            return 0;
        }

        n1 = 1;
        InterpName = cstk(l1);
        CreateVar(Rhs + 1, MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &l1);
        *istk(l1) = (int) existsSlaveInterp(InterpName);

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
