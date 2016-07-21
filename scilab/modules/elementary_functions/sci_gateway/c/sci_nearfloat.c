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
#include "api_scilab.h"
#include "Scierror.h"


int sci_nearfloat(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int i;

    int* piAddr1				= NULL;
    int iRows1					= 0;
    int iCols1					= 0;
    int iType1					= 0;
    int* piLen					= NULL;
    char **pstData			= NULL;

    int* piAddr2				= NULL;
    int iRows2					= 0;
    int iCols2					= 0;
    int iType2					= 0;
    double *pdblReal		= NULL;

    double dblMode			= 0;

    double *pdblRealRet	= NULL;


    CheckRhs(2, 2);
    CheckLhs(1, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddr1, &iType1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddr2, &iType2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (iType1 != sci_strings)
    {
        //Err = 1;
        SciError(55);
        return 0;
    }

    if (iType2 != sci_matrix)
    {
        //Err = 2;
        SciError(53);
        return 0;
    }

    sciErr = getMatrixOfString(pvApiCtx, piAddr1, &iRows1, &iCols1, NULL, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if (iRows1 != 1 || iCols1 != 1)
    {
        return 1;
    }

    piLen = (int*)malloc(sizeof(int) * iRows1 * iCols1);
    sciErr = getMatrixOfString(pvApiCtx, piAddr1, &iRows1, &iCols1, piLen, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        free(piLen);
        return 0;
    }

    pstData	= (char**)malloc(sizeof(char*) * iRows1 * iCols1);
    for (i = 0 ; i < iRows1 * iCols1 ; i++)
    {
        pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+1 for null termination
    }

    sciErr = getMatrixOfString(pvApiCtx, piAddr1, &iRows1, &iCols1, piLen, pstData);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        free(piLen);
        for (i = 0 ; i < iRows1 * iCols1 ; i++)
        {
            free(pstData[i]);
        }
        free(pstData);
        return 0;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows2, &iCols2, &pdblReal);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        free(piLen);
        for (i = 0 ; i < iRows1 * iCols1 ; i++)
        {
            free(pstData[i]);
        }
        free(pstData);
        return 0;
    }

    if (strcmp(pstData[0], "succ") == 0)
    {
        dblMode = 1.0;
    }
    else if (strcmp(pstData[0], "pred") == 0)
    {
        dblMode = -1.0;
    }
    else
    {
        SciError(999);
        free(piLen);
        for (i = 0 ; i < iRows1 * iCols1 ; i++)
        {
            free(pstData[i]);
        }
        free(pstData);
        return 0;
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iRows2, iCols2, &pdblRealRet);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        free(piLen);
        for (i = 0 ; i < iRows1 * iCols1 ; i++)
        {
            free(pstData[i]);
        }
        free(pstData);
        return 0;
    }

    for (i = 0 ; i < iRows2 * iCols2 ; i++)
    {
        pdblRealRet[i] = dblNearFloat(pdblReal[i], dblMode);
    }

    free(piLen);
    for (i = 0 ; i < iRows1 * iCols1 ; i++)
    {
        free(pstData[i]);
    }
    free(pstData);

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
