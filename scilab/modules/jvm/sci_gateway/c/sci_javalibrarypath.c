/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) Scilab Enterprises - 2015 - Vincent COUVERT
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

/*--------------------------------------------------------------------------*/
#include "gw_jvm.h"
#include "sci_malloc.h"
#include "Scierror.h"
#include "addToLibrarypath.h"
#include "getLibrarypath.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int sci_javalibrarypath(char *fname, void* pvApiCtx)
{
    int *piAddressVarOne = NULL;
    int iType = 0;
    SciErr sciErr;

    CheckRhs(0, 1);
    CheckLhs(0, 1);

    if (Rhs == 0)
    {
        int iRows = 0;
        int iCols = 1;
        char **pstLibrarypath = NULL;

        pstLibrarypath = getLibrarypath(&iRows);
        createMatrixOfString(pvApiCtx, Rhs + 1, iRows, iCols, pstLibrarypath);

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        freeArrayOfString(pstLibrarypath, iRows * iCols);
    }
    else
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if ( iType == sci_strings )
        {
            char **pStVarOne = NULL;
            static int iCols = 0, iRows = 0;
            int i = 0;

            if (getAllocatedMatrixOfString(pvApiCtx, piAddressVarOne, &iRows, &iCols, &pStVarOne))
            {
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }
            for (i = 0; i < iRows * iCols ; i++)
            {
                if (!addToLibrarypath(pStVarOne[i]))
                {
                    Scierror(999, _("%s: Could not add path to java.library.path: %s.\n"), fname, pStVarOne[i]);;
                    freeArrayOfString(pStVarOne, iRows * iCols);
                    return 0;
                }
            }
            LhsVar(1) = 0;
            PutLhsVar();
            freeArrayOfString(pStVarOne, iRows * iCols);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
        }
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
