/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include "gw_windows_tools.h"
#include "stack-c.h"
#include "winopen.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_winopen(char *fname, unsigned long l)
{
    int m1 = 0, n1 = 0, l1 = 0;

    CheckRhs(1, 1);

    GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);

    if (winopen(cstk(l1)))
    {
        LhsVar(1) = 0;
    }
    else
    {
        Scierror(999, _("%s: Cannot open file %s.\n"), fname, cstk(l1));
        return 0;
    }

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
