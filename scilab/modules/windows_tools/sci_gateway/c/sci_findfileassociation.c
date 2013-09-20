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
#include <windows.h>
#include <string.h>
#include "gw_windows_tools.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "FindFileAssociation.h"
#include "PATH_MAX.h"
#include "stack-c.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_findfileassociation(char *fname, unsigned long l)
{
    static int l1, n1, m1;
    char *Output = NULL;
    char ExtraParam[PATH_MAX];
    char AssocParam[PATH_MAX];

    Rhs = Max(Rhs, 0);
    CheckRhs(1, 2);
    CheckLhs(0, 1);

    if (Rhs == 1)
    {
        if (GetType(1) != sci_strings)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n") , fname, 1);
            return 0;
        }
        else
        {
            char *param = NULL;

            GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
            param = cstk(l1);
            strcpy(AssocParam, param);

            strcpy(ExtraParam, "open");
        }
    }
    else
    {
        if ( (GetType(1) != sci_strings) && (GetType(2) != sci_strings) )
        {
            Scierror(999, _("%s: Wrong type for input arguments: Strings expected.\n"), fname);
            return 0;
        }
        else
        {
            char *param = NULL;

            GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
            param = cstk(l1);

            strcpy(AssocParam, param);

            GetRhsVar(2, STRING_DATATYPE, &m1, &n1, &l1);
            param = cstk(l1);

            strcpy(ExtraParam, param);
        }
    }

    Output = FindFileAssociation(AssocParam, ExtraParam) ;

    if (Output)
    {
        n1 = 1;
        m1 = (int)strlen(Output);
        CreateVarFromPtr(Rhs + 1, STRING_DATATYPE, &m1, &n1, &Output);
    }
    else
    {
        n1 = 0;
        m1 = 0;
        l1 = 0;
        CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &n1, &m1, &l1);
    }

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
