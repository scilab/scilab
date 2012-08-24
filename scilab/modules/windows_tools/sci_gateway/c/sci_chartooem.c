/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <windows.h>
#include "gw_windows_tools.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "scilabmode.h"
#include "localization.h"
#include "Scierror.h"
#include "warningmode.h"
#include "sciprint.h"
/*--------------------------------------------------------------------------*/
int sci_chartooem(char *fname, unsigned long l)
{
    static int l1, n1, m1;
    char *Output = NULL;

    if (getWarningMode())
    {
        sciprint(_("%s: Feature %s is obsolete.\n"), _("Warning"), fname);
        sciprint(_("%s: This feature will be permanently removed in Scilab %s\n\n"), _("Warning"), "5.4.1");
    }

    CheckRhs(1, 1);
    CheckLhs(0, 1);

    if (GetType(1) == sci_strings)
    {
        char *Charstring = NULL;

        GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
        Charstring = cstk(l1);

        Output = (char*)MALLOC((strlen(Charstring) + 1) * sizeof(char));
        if (getScilabMode() == SCILAB_STD)
        {
            CharToOem(Charstring, Output);
        }
        else
        {
            wsprintf(Output, "%s", Charstring);
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
        return 0;
    }

    n1 = 1;
    m1 = (int)strlen(Output);
    CreateVarFromPtr( Rhs + 1, STRING_DATATYPE, &m1, &n1, &Output);
    if (Output)
    {
        FREE(Output);
        Output = NULL;
    }
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
