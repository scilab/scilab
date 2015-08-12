/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2011 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_windows_tools.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int sci_win64(char *fname, unsigned long l)
{
    BOOL Status = FALSE;

    CheckRhs(0, 0);
    CheckLhs(0, 1);

#ifdef _WIN64
    Status = TRUE;
#endif

    if (createScalarBoolean(pvApiCtx, Rhs + 1, Status) != 0)
    {
        return 1;
    }

    LhsVar(1) = Rhs + 1;

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/

