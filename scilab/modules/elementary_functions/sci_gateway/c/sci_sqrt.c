/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_elementary_functions.h"
#include "basic_functions.h"
#include "api_scilab.h"


int sci_sqrt(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int i;
    int iRet						= 0;
    int iComplex				= 0;
    int iRows						= 0;
    int iCols						= 0;
    int* piAddr					= NULL;

    double* pdblReal		= NULL;
    double* pdblImg			= NULL;
    double* pdblRealRet	= NULL;
    double* pdblImgRet	= NULL;


    CheckRhs(1, 1);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (!isDoubleType(pvApiCtx, piAddr))
    {
        OverLoad(1);
        return 0;
    }

    if (isVarComplex(pvApiCtx, piAddr))
    {
        iComplex = 1;
        sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal, &pdblImg);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }
    else
    {
        iComplex = 0;
        sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }

    for (i = 0 ; i < iRows * iCols && iComplex == 0 ; i++)
    {
        if (pdblReal[i] < 0)
        {
            iComplex = 1;
        }
    }

    if (iComplex)
    {
        sciErr = allocComplexMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet, &pdblImgRet);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if (isVarComplex(pvApiCtx, piAddr))
        {
            for (i = 0 ; i < iRows * iCols ; i++)
            {
                zsqrts(pdblReal[i], pdblImg[i], &pdblRealRet[i], &pdblImgRet[i]);
            }
        }
        else
        {
            for (i = 0 ; i < iRows * iCols ; i++)
            {
                zsqrts(pdblReal[i], 0, &pdblRealRet[i], &pdblImgRet[i]);
            }
        }
    }
    else
    {
        sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        for (i = 0 ; i < iRows * iCols ; i++)
        {
            pdblRealRet[i] = dsqrts(pdblReal[i]);
        }
    }
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
