/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2012 - Allan CORNET
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
#include <stdio.h>
#include <string.h>
#include "gw_time.h"
#include "sci_malloc.h"
#include "getdate.h"
#include "Scierror.h"
#include "localization.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int sci_getdate(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    Rhs = Max(Rhs, 0);
    CheckRhs(0, 1) ;
    CheckLhs(0, 1) ;

    if (Rhs == 0)
    {
        int iErr = 0;
        double *dDate = getCurrentDateAsDoubleVector(&iErr);

        if (iErr)
        {
            Scierror(999, _("%s: An error occurred.\n"), fname);
            if (dDate)
            {
                FREE(dDate);
                dDate = NULL;
            }
            return 0;
        }

        if (dDate)
        {
            sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, NB_ELEMNT_ARRAY_GETDATE, dDate);
            FREE(dDate);
            dDate = NULL;

            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }
    else /* Rhs == 1 */
    {
        int *piAddressVarOne = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
            return 0;
        }

        if (isStringType(pvApiCtx, piAddressVarOne))
        {
            if (isScalar(pvApiCtx, piAddressVarOne))
            {
                double dTime = 0.;

                char *pStr = NULL;
                if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &pStr) != 0)
                {
                    Scierror(999, _("%s: No more memory.\n"), fname);
                    return 0;
                }

                if (strcmp(pStr, "s") != 0)
                {
                    freeAllocatedSingleString(pStr);
                    pStr = NULL;
                    Scierror(999, _("%s: Wrong value for input argument #%d: '%s' expected.\n"), fname, 1, "s");
                    return 0;
                }

                freeAllocatedSingleString(pStr);
                pStr = NULL;

                dTime = getCurrentDateAsUnixTimeConvention();

                if (createScalarDouble(pvApiCtx, Rhs + 1, dTime) != 0)
                {
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 0;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 1);
                return 0;
            }
        }
        else if (isDoubleType(pvApiCtx, piAddressVarOne))
        {
            if (isEmptyMatrix(pvApiCtx, piAddressVarOne))
            {
                if (createEmptyMatrix(pvApiCtx, Rhs + 1) != 0)
                {
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 0;
                }
            }
            else if (!isVarComplex(pvApiCtx, piAddressVarOne))
            {
                int iErr = 0;
                double *dValues = NULL;
                double *dResults = NULL;
                int m = 0, n = 0;
                int nbElements = 0;
                int i = 0;
                sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &m, &n, &dValues);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
                    return 0;
                }

                nbElements = m * n;

                for (i = 0; i < nbElements; i++)
                {
                    if (dValues[i] < 0.)
                    {
                        Scierror(999, _("%s: Wrong value for input argument #%d: Must be > %d.\n"), fname, 1, 0);
                        return 0;
                    }
                }

                dResults = getConvertedDateAsMatrixOfDouble(dValues, nbElements, &iErr);

                if (iErr == 2)
                {
                    FREE(dResults);
                    Scierror(999, _("%s: An error occurred.\n"), fname);
                    return 0;
                }

                if (dResults == NULL)
                {
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 0;
                }


                sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, nbElements, NB_ELEMNT_ARRAY_GETDATE, dResults);

                FREE(dResults);
                dResults = NULL;

                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 0;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A real expected.\n"), fname, 1);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Integer or '%s' expected.\n"), fname, 1, "s");
            return 0;
        }
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/

