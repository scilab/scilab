/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007-2008 - INRIA - Allan CORNET
* Copyright (C) 2011 - DIGITEO - Allan CORNET
* Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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

/*------------------------------------------------------------------------*/
#include "gw_history_manager.h"
#include "HistoryManager.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_historysize(char *fname, int* pvApiCtx)
{
    int iReturnedValue = 0;

    Rhs = Max(Rhs, 0);
    CheckRhs(0, 1);
    CheckLhs(0, 1);
    if (Rhs == 1)
    {
        int iValue = 0;
        int *piAddressVarOne = NULL;
        SciErr sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
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
                char *paramValue = NULL;
                if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &paramValue) == 0)
                {
                    if (strcmp(paramValue, "max") == 0)
                    {
                        iReturnedValue = getSizeMaxScilabHistory();
                        freeAllocatedSingleString(paramValue);
                        paramValue = NULL;
                    }
                    else
                    {
                        Scierror(999, _("%s: Wrong value for input argument #%d: \"%s\" expected.\n"), fname, 1, "max");
                        freeAllocatedSingleString(paramValue);
                        paramValue = NULL;
                        return 0;
                    }
                }
                else
                {
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 0;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 1);
                return 0;
            }
        }
        else if (isDoubleType(pvApiCtx, piAddressVarOne))
        {
            if (isScalar(pvApiCtx, piAddressVarOne))
            {
                double dValue = 0.;
                if (getScalarDouble(pvApiCtx, piAddressVarOne, &dValue) == 0)
                {
                    iValue = (int)(dValue);
                }

                if (!setSizeMaxScilabHistory(iValue))
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d.\n"), fname, 1);
                    return 0;
                }
                iReturnedValue = iValue;
            }
            else
            {
                Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 1);
                return 0;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d.\n"), fname, 1);
            return 0;
        }
    }
    else
    {
        iReturnedValue = getSizeScilabHistory();
    }

    createScalarDouble(pvApiCtx, Rhs + 1, (double)iReturnedValue);
    LhsVar(1) = Rhs + 1;

    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
