/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "Scierror.h"
#include "stack-c.h"
#include "do_xxprintf.h"
#include "gw_fileio.h"
#include "filesmanagement.h"
#include "localization.h"
#include "BOOL.h"
#include "scilabmode.h"
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static BOOL forceSTDERRredirect = TRUE;
#endif
/*--------------------------------------------------------------------------*/
int sci_mfprintf(char *fname, unsigned long fname_len)
{
    FILE *f;
    BOOL isSTD = FALSE;
    int fileMode = 0;
    int l1 = 0, m1 = 0, n1 = 0;
    int l2 = 0, m2 = 0, n2 = 0;
    int lcount = 0, rval = 0, mx = 0, mk = 0, nk = 0, k = 0;
    char *ptrFormat = NULL;
    int lenptrFormat = 0;
    int i = 0;
    int NumberPercent = 0;
    int param1 = 0;

    Nbvars = 0;
    CheckRhs(1, 1000);
    CheckLhs(0, 1);
    if ( Rhs < 2 )
    {
        Scierror(999, _("%s: Wrong number of input arguments: At least %d expected.\n"), fname, 2);
        return 0;
    }
    for ( k = 3; k <= Rhs; k++)
    {
        if (VarType(k) != sci_matrix && VarType(k) != sci_strings)
        {
            OverLoad(k);
            return 0;
        }
    }
    GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &l1); /* file id */
    GetRhsVar(2, STRING_DATATYPE, &m2, &n2, &l2); /* format */
    ptrFormat = cstk(l2);

    param1 = *istk(l1);

    switch (param1)
    {
        case 0:
#ifdef _MSC_VER
            if ( (getScilabMode()  == SCILAB_STD) && (forceSTDERRredirect == TRUE) )
            {
                /*  Console redirect stderr --> CONOUT$*/
                freopen("CONOUT$", "wb", stderr);
                forceSTDERRredirect = FALSE;
            }
#endif
            f = stderr;
            break;
        case 5:
            // stdin;
            f = (FILE *)0;
            break;
        case 6:
            f = stdout;
            break;
        default:
            f = GetFileOpenedInScilab(param1);
            break;
    }

    if (f == (FILE *)0 )
    {
        Scierror(999, _("%s: Wrong file descriptor: %d.\n"), fname, *istk(l1));
        return 0;
    }

    switch (param1)
    {
        case 0:
        case 6:
            isSTD = TRUE;
            break;
        default:
            isSTD = FALSE;
            fileMode = GetFileModeOpenedInScilab(param1);
            break;
    }


    /* checks file mode */
    /* bug 3898 */
    /* read only attrib 1xx*/
    if ( (fileMode >= 100) && (fileMode < 200) && ((fileMode % 100) < 10) /* check that it is not r+ */ &&  !isSTD)
    {
        Scierror(999, _("%s: Wrong file mode: READ only.\n"), fname);
        return 0;
    }

    lenptrFormat = (int)strlen(ptrFormat);
    for (i = 0; i < lenptrFormat; i++)
    {
        if (ptrFormat[i] == '%')
        {
            NumberPercent++;
            if (ptrFormat[i + 1] == '%')
            {
                NumberPercent--;
                i++;
            }
        }
    }

    if (NumberPercent < Rhs - 2)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), fname, NumberPercent + 2);
        return 0;
    }

    mx = 0;
    if (Rhs >= 3)
    {
        GetMatrixdims(3, &mx, &nk);
        for (k = 4; k <= Rhs; k++)
        {
            GetMatrixdims(k, &mk, &nk);
            mx = Min(mx, mk);
        }
    }
    lcount = 1;
    if (Rhs == 2)
    {
        rval = do_xxprintf("fprintf", f, cstk(l2), Rhs, 2, lcount, (char **)0);
    }
    else while (1)
        {
            if ((rval = do_xxprintf("fprintf", f, cstk(l2), Rhs, 2, lcount, (char **)0)) < 0)
            {
                break;
            }
            lcount++;
            if (lcount > mx)
            {
                break;
            }
        }
    if (rval == RET_BUG)
    {
        return 0;
    }
    LhsVar(1) = 0; /** No return value **/
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
