/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
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
#include <string.h>
#include "gw_elementary_functions.h"
#include "basic_functions.h"
#include "sciprint.h"
#include "localization.h"
#include "api_scilab.h"
#include "Scierror.h"

/*--------------------------------------------------------------------------*/
int sci_number_properties(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int i;
    int iRet = 0;

    int iRows = 0;
    int iCols = 0;
    char **pstData = NULL;
    int* piAddr = NULL;

    double dblRet = 0;
    int bRet = 0;

    int bBoolFlag = 0;

    CheckRhs(1, 1);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    iRet = getAllocatedMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, &pstData);
    if (iRet)
    {
        freeAllocatedMatrixOfString(iRows, iCols, pstData);
        return 1;
    }

    if (strcmp(pstData[0], "eps") == 0)
    {
        dblRet = nc_eps();
    }
    else if (strcmp(pstData[0], "huge") == 0)
    {
        dblRet = nc_double_max();
    }
    else if (strcmp(pstData[0], "tiny") == 0)
    {
        dblRet = nc_double_min();
    }
    else if (strcmp(pstData[0], "radix") == 0)
    {
        dblRet = nc_base();
    }
    else if (strcmp(pstData[0], "digits") == 0)
    {
        dblRet = nc_num_mantissa_digits();
    }
    else if (strcmp(pstData[0], "minexp") == 0)
    {
        dblRet = nc_exp_min();
    }
    else if (strcmp(pstData[0], "maxexp") == 0)
    {
        dblRet = nc_exp_max();
    }
    else if (strcmp(pstData[0], "denorm") == 0)
    {
        bBoolFlag = 1;
        if (nc_double_min() / nc_base() > 0)
        {
            bRet	= 1;
        }
        else
        {
            bRet	= 0;
        }
    }
    else if (strcmp(pstData[0], "tiniest") == 0)
    {
        double dblRadix = nc_base();
        dblRet = nc_double_min();

        if (dblRet	/ dblRadix != 0)
        {
            //denormalised number are used
            int iDigits = (int)nc_num_mantissa_digits();
            for (i = 1 ; i < iDigits ; i++)
            {
                dblRet	/= dblRadix;
            }
        }
    }
    else
    {
        freeAllocatedMatrixOfString(iRows, iCols, pstData);
        sciprint(_("%s: unknown property kind.\n"), fname);
        SciError(999);
        return 0;
    }

    freeAllocatedMatrixOfString(iRows, iCols, pstData);

    if (bBoolFlag)
    {
        iRet = createScalarBoolean(pvApiCtx, Rhs + 1, bRet);
    }
    else
    {
        iRet = createScalarDouble(pvApiCtx, Rhs + 1, dblRet);
    }

    if (iRet)
    {
        return 1;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
