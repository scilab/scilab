/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2011 - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "gw_jvm.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "addToClasspath.h"
#include "getClasspath.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int sci_javaclasspath(char *fname, unsigned long fname_len)
{
    int *piAddressVarOne = NULL;
    int iType = 0;
    SciErr sciErr;

    Rhs = Max(Rhs, 0);
    CheckRhs(0, 1);
    CheckLhs(0, 1);

    if (Rhs == 0)
    {
        int nbRow = 0;
        int nbCol = 1;
        char **Strings = NULL;

        Strings = getClasspath(&nbRow);
        createMatrixOfString(pvApiCtx, Rhs + 1, nbRow, nbCol, Strings);

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        freeArrayOfString(Strings, nbRow * nbCol);
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
            int *lenStVarOne = NULL;
            static int n1 = 0, m1 = 0;
            int i = 0;

            /* get dimensions */
            sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

            lenStVarOne = (int*)MALLOC(sizeof(int) * (m1 * n1));
            if (lenStVarOne == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }

            /* get lengths */
            sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
            if (sciErr.iErr)
            {
                if (lenStVarOne)
                {
                    FREE(lenStVarOne);
                    lenStVarOne = NULL;
                }
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

            pStVarOne = (char **)MALLOC(sizeof(char*) * (m1 * n1));
            if (pStVarOne == NULL)
            {
                if (lenStVarOne)
                {
                    FREE(lenStVarOne);
                    lenStVarOne = NULL;
                }
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }
            for (i = 0; i < m1 * n1; i++)
            {
                pStVarOne[i] = (char*)MALLOC(sizeof(char*) * (lenStVarOne[i] + 1));
            }

            /* get strings */
            sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, pStVarOne);
            if (sciErr.iErr)
            {
                freeArrayOfString(pStVarOne, m1 * n1);
                if (lenStVarOne)
                {
                    FREE(lenStVarOne);
                    lenStVarOne = NULL;
                }
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                return 0;
            }

            for (i = 0; i < m1 * n1 ; i++)
            {
                if (!addToClasspath(pStVarOne[i], STARTUP))
                {
                    Scierror(999, _("%s: Could not add URL to system classloader : %s.\n"), fname, pStVarOne[i]);
                    freeArrayOfString(pStVarOne, m1 * n1);
                    return 0;
                }
            }
            LhsVar(1) = 0;
            PutLhsVar();
            freeArrayOfString(pStVarOne, m1 * n1);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
        }
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
