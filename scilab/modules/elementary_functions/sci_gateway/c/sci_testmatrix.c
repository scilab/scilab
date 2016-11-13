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
#include "gw_elementary_functions.h"
#include "basic_functions.h"
#include "Scierror.h"
#include "api_scilab.h"

#define MAGI_LETTER		'm'
#define FRK_LETTER		'f'
#define HILB_LETTER		'h'

char getGenerateMode(void* pvApiCtx, int* _piAddress);
int getGenerateSize(void* pvApiCtx, int* _piAddress);
/*--------------------------------------------------------------------------*/
int sci_testmatrix(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int iRet						= 0;

    int iRows1					= 0;
    int iCols1					= 0;
    int* piAddr1				= NULL;
    char cMode					= 0;


    int iRows2					= 0;
    int iCols2					= 0;
    int* piAddr2				= NULL;
    int iDim						= 0;

    double *pdblRealRet = NULL;

    CheckRhs(2, 2);
    CheckLhs(1, 1);

    /*check input 1*/
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

    cMode = getGenerateMode(pvApiCtx, piAddr1);

    if (cMode == -1)
    {
        return 1;
    }

    iDim = getGenerateSize(pvApiCtx, piAddr2);


    if (cMode != FRK_LETTER && cMode != HILB_LETTER && iDim == 2)
    {
        iDim = 0;
    }

    if (iDim == 0)
    {
        iRet = createEmptyMatrix(pvApiCtx, Rhs + 1);
        if (iRet)
        {
            return 1;
        }
        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        return 0;
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iDim, iDim, &pdblRealRet);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    switch (cMode)
    {
        case FRK_LETTER :
            franck_matrix(iDim, pdblRealRet);
            break;
        case HILB_LETTER :
            hilb_matrix(iDim, pdblRealRet);
            break;
        default : //Magic case and others
            magic_matrix(iDim, pdblRealRet);
            break;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}

char getGenerateMode(void* pvApiCtx, int* _piAddress)
{
    int iRet = 0;

    char* pstData;
    char ret;
    iRet = getAllocatedSingleString(pvApiCtx, _piAddress, &pstData);
    if (iRet)
    {
        return -1;
    }
    ret = pstData[0];
    freeAllocatedSingleString(pstData);
    return ret;
}

int getGenerateSize(void* pvApiCtx, int* _piAddress)
{
    SciErr sciErr;
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;

    double* pdblReal = NULL;
    double* pdblImg	 = NULL;

    if (isVarComplex(pvApiCtx, _piAddress))
    {
        sciErr = getComplexMatrixOfDouble(pvApiCtx, _piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }
    else
    {
        sciErr = getMatrixOfDouble(pvApiCtx, _piAddress, &iRows, &iCols, &pdblReal);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
    }
    return abs((int)pdblReal[0]);

}
/*--------------------------------------------------------------------------*/
