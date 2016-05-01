/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2011 - DIGITEO - Antoine ELIAS
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
#include <string.h>
#include <stdio.h>
#include "api_scilab.h"
#include "BOOL.h"
#include "sci_malloc.h"
#include "serialization.h"
#include "elem_common.h"

static int serialize_double(void *_pvCtx, int *_piAddr, int **_piBuffer, int *_piBufferSize)
{
    SciErr sciErr;
    int iRows = 0;
    int iCols = 0;
    int iOne = 1;
    int iSize = 0;
    double *pdblR = NULL;
    double *pdblI = NULL;

    int *piOut = NULL;
    int iOutLen = 0;

    if (isVarComplex(_pvCtx, _piAddr))
    {
        double *p = NULL;

        sciErr = getComplexMatrixOfDouble(_pvCtx, _piAddr, &iRows, &iCols, &pdblR, &pdblI);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        iOutLen = 4 + (2 * iRows * iCols * sizeof(double) / sizeof(int));
        piOut = (int *)MALLOC(iOutLen * sizeof(int));
        if (piOut == NULL)
        {
            return 1;
        }

        piOut[0] = sci_matrix;
        piOut[1] = iRows;
        piOut[2] = iCols;
        piOut[3] = 1;           //complex

        //move 'p' to first real value
        p = (double *)(piOut + 4);
        iSize = iRows * iCols;
        C2F(dcopy) (&iSize, pdblR, &iOne, p, &iOne);
        //move 'p' to first complex value
        p = p + iRows * iCols;
        C2F(dcopy) (&iSize, pdblI, &iOne, p, &iOne);
    }
    else
    {
        double *p = NULL;

        sciErr = getMatrixOfDouble(_pvCtx, _piAddr, &iRows, &iCols, &pdblR);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        iOutLen = 4 + (iRows * iCols * sizeof(double) / sizeof(int));
        piOut = (int *)MALLOC(iOutLen * sizeof(int));
        if (piOut == NULL)
        {
            return 1;
        }

        piOut[0] = sci_matrix;
        piOut[1] = iRows;
        piOut[2] = iCols;
        piOut[3] = 0;           //not complex

        //move 'p' to first value
        p = (double *)(piOut + 4);
        iSize = iRows * iCols;
        C2F(dcopy) (&iSize, pdblR, &iOne, p, &iOne);
    }

    *_piBuffer = piOut;
    *_piBufferSize = iOutLen;
    return 0;
}

static int serialize_string(void *_pvCtx, int *_piAddr, int **_piBuffer, int *_piBufferSize)
{
    int iErr = 0;
    int i = 0;
    int iRows = 0;
    int iCols = 0;
    char **pstData = NULL;
    char *p = NULL;

    int *piOut = NULL;
    int *piOutLen = NULL;
    int iOutLen = 0;

    iErr = getAllocatedMatrixOfString(_pvCtx, _piAddr, &iRows, &iCols, &pstData);
    if (iErr)
    {
        return 1;
    }

    for (i = 0; i < iRows * iCols; i++)
    {
        iOutLen += (int)strlen(pstData[i]);
    }

    if (iOutLen % 4)
    {
        iOutLen = iOutLen / (sizeof(int) / sizeof(char)) + 1;
    }
    else
    {
        iOutLen = iOutLen / (sizeof(int) / sizeof(char));
    }

    //4 for header and size of each string
    iOutLen += 4 + iRows * iCols;

    piOut = (int *)MALLOC(iOutLen * sizeof(int));
    if (piOut == NULL)
    {
        return 1;
    }

    piOut[0] = sci_strings;
    piOut[1] = iRows;
    piOut[2] = iCols;
    piOut[3] = 0;               //not complex

    piOutLen = piOut + 4;

    for (i = 0; i < iRows * iCols; i++)
    {
        piOutLen[i] = (int)strlen(pstData[i]);
    }

    p = (char *)(piOut + 4 + iRows * iCols);
    for (i = 0; i < iRows * iCols; i++)
    {
        memcpy(p, pstData[i], piOutLen[i]);
        p += piOutLen[i];
    }

    *_piBuffer = piOut;
    *_piBufferSize = iOutLen;

    freeAllocatedMatrixOfString(iRows, iCols, pstData);
    return 0;
}

static int serialize_boolean(void *_pvCtx, int *_piAddr, int **_piBuffer, int *_piBufferSize)
{
    SciErr sciErr;
    int iRows = 0;
    int iCols = 0;
    int *piBool = NULL;
    int *p = NULL;

    int *piOut = NULL;
    int iOutLen = 0;

    sciErr = getMatrixOfBoolean(_pvCtx, _piAddr, &iRows, &iCols, &piBool);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    //4 for header and 1 for each boolean
    iOutLen = 4 + iRows * iCols;

    piOut = (int *)MALLOC(iOutLen * sizeof(int *));
    if (piOut == NULL)
    {
        return 1;
    }

    piOut[0] = sci_boolean;
    piOut[1] = iRows;
    piOut[2] = iCols;
    piOut[3] = 0;               //not complex

    p = (int *)(piOut + 4);
    memcpy(p, piBool, iRows * iCols * sizeof(int));

    *_piBuffer = piOut;
    *_piBufferSize = iOutLen;
    return 0;
}

static int serialize_int(void *_pvCtx, int *_piAddr, int **_piBuffer, int *_piBufferSize)
{
    SciErr sciErr;
    int iPrecision = 0;
    int iRows = 0;
    int iCols = 0;
    int iItemSize = 0;
    void *p = NULL;
    void *pvData = NULL;

    int *piOut = NULL;
    int iOutLen = 0;

    sciErr = getVarDimension(_pvCtx, _piAddr, &iRows, &iCols);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    sciErr = getMatrixOfIntegerPrecision(_pvCtx, _piAddr, &iPrecision);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    if (iPrecision == SCI_INT8 || iPrecision == SCI_UINT8)
    {
        iItemSize = sizeof(char);
    }
    else if (iPrecision == SCI_INT16 || iPrecision == SCI_UINT16)
    {
        iItemSize = sizeof(short);
    }
    else if (iPrecision == SCI_INT32 || iPrecision == SCI_UINT32)
    {
        iItemSize = sizeof(int);
    }
    /*
        else if(iPrecision == SCI_INT64 || iPrecision == SCI_UINT64)
        {
            iItemSize = sizeof(long long);
        }
    */
    //check and adjust alignement on integer
    iOutLen = iRows * iCols;
    if ((iOutLen * iItemSize) % sizeof(int))
    {
        iOutLen = (iOutLen * iItemSize) / sizeof(int) + 1;
    }
    else
    {
        iOutLen = (iOutLen * iItemSize) / (sizeof(int));
    }

    iOutLen += 4;
    piOut = (int *)MALLOC(iOutLen * sizeof(int *));
    if (piOut == NULL)
    {
        return 1;
    }

    piOut[0] = sci_ints;
    piOut[1] = iRows;
    piOut[2] = iCols;
    piOut[3] = iPrecision;      //precision

    switch (iPrecision)
    {
        case SCI_INT8:
        {
            sciErr = getMatrixOfInteger8(_pvCtx, _piAddr, &iRows, &iCols, (char **)&pvData);
            break;
        }
        case SCI_UINT8:
        {
            sciErr = getMatrixOfUnsignedInteger8(_pvCtx, _piAddr, &iRows, &iCols, (unsigned char **)&pvData);
            break;
        }
        case SCI_INT16:
        {
            sciErr = getMatrixOfInteger16(_pvCtx, _piAddr, &iRows, &iCols, (short **)&pvData);
            break;
        }
        case SCI_UINT16:
        {
            sciErr = getMatrixOfUnsignedInteger16(_pvCtx, _piAddr, &iRows, &iCols, (unsigned short **)&pvData);
            break;
        }
        case SCI_INT32:
        {
            sciErr = getMatrixOfInteger32(_pvCtx, _piAddr, &iRows, &iCols, (int **)&pvData);
            break;
        }
        case SCI_UINT32:
        {
            sciErr = getMatrixOfUnsignedInteger32(_pvCtx, _piAddr, &iRows, &iCols, (unsigned int **)&pvData);
            break;
        }
        /*
            case SCI_INT64 :
            {
                sciErr = getMatrixOfInteger64(_pvCtx, _piAddr, &iRows, &iCols, (long long**)&pvData);
                break;
            }
            case SCI_UINT64 :
            {
                sciErr = getMatrixOfUnsignedInteger64(_pvCtx, _piAddr, &iRows, &iCols, (unsigned long long**)&pvData);
                break;
                }
    */ default:
            FREE(piOut);
            return 1;
    }

    if (sciErr.iErr)
    {
        FREE(piOut);
        printError(&sciErr, 0);
        return 1;
    }

    p = piOut + 4;
    memcpy(p, pvData, iRows * iCols * iItemSize);
    *_piBuffer = piOut;
    *_piBufferSize = iOutLen;
    return 0;
}

static int serialize_sparse(void *_pvCtx, int *_piAddr, int **_piBuffer, int *_piBufferSize, BOOL _bData)
{
    int iRet = 0;
    int iRows = 0;
    int iCols = 0;
    int iItemCount = 0;
    int *piRowCount = 0;
    int *piColPos = 0;
    int iComplex = 0;

    double *pdblR = NULL;
    double *pdblI = NULL;

    int *piOut = NULL;
    int iOutLen = 0;

    if (_bData)
    {
        //sparse

        iComplex = isVarComplex(_pvCtx, _piAddr);
        if (iComplex)
        {
            iRet = getAllocatedComplexSparseMatrix(_pvCtx, _piAddr, &iRows, &iCols, &iItemCount, &piRowCount, &piColPos, &pdblR, &pdblI);
        }
        else
        {
            iRet = getAllocatedSparseMatrix(_pvCtx, _piAddr, &iRows, &iCols, &iItemCount, &piRowCount, &piColPos, &pdblR);
        }
    }
    else
    {
        //boolean sparse
        iRet = getAllocatedBooleanSparseMatrix(_pvCtx, _piAddr, &iRows, &iCols, &iItemCount, &piRowCount, &piColPos);
    }

    if (iRet)
    {
        return 1;
    }

    //5 -> 4 for header + 1 for item count
    iOutLen = 5 + iRows + iItemCount;

    if (_bData)
    {
        iOutLen += iItemCount * (iComplex + 1) * sizeof(double) / sizeof(int);
    }

    piOut = (int *)MALLOC(iOutLen * sizeof(int));

    piOut[0] = _bData ? sci_sparse : sci_boolean_sparse;
    piOut[1] = iRows;
    piOut[2] = iCols;
    piOut[3] = iComplex;
    piOut[4] = iItemCount;

    memcpy(piOut + 5, piRowCount, iRows * sizeof(int));
    memcpy(piOut + 5 + iRows, piColPos, iItemCount * sizeof(int));

    if (_bData)
    {
        int iOne = 1;
        double *pRealData = (double *)(piOut + 5 + iRows + iItemCount);

        C2F(dcopy) (&iItemCount, pdblR, &iOne, pRealData, &iOne);
        if (iComplex)
        {
            double *pImgData = pRealData + iItemCount;

            C2F(dcopy) (&iItemCount, pdblI, &iOne, pImgData, &iOne);
        }
    }

    *_piBuffer = piOut;
    *_piBufferSize = iOutLen;

    if (_bData)
    {
        if (iComplex)
        {
            freeAllocatedSparseMatrix(piRowCount, piColPos, pdblR);
        }
        else
        {
            freeAllocatedComplexSparseMatrix(piRowCount, piColPos, pdblR, pdblI);
        }
    }
    else
    {
        freeAllocatedBooleanSparse(piRowCount, piColPos);
    }

    return 0;
}

int serialize_to_mpi(void *_pvCtx, int *_piAddr, int **_piBuffer, int *_piBufferSize)
{
    int iType = 0;
    SciErr sciErr = getVarType(_pvCtx, _piAddr, &iType);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    switch (iType)
    {
        case sci_matrix:
            return serialize_double(_pvCtx, _piAddr, _piBuffer, _piBufferSize);
            break;
        case sci_strings:
            return serialize_string(_pvCtx, _piAddr, _piBuffer, _piBufferSize);
            break;
        case sci_boolean:
            return serialize_boolean(_pvCtx, _piAddr, _piBuffer, _piBufferSize);
            break;
        case sci_sparse:
            return serialize_sparse(_pvCtx, _piAddr, _piBuffer, _piBufferSize, TRUE);
            break;
        case sci_boolean_sparse:
            return serialize_sparse(_pvCtx, _piAddr, _piBuffer, _piBufferSize, FALSE);
            break;
        case sci_ints:
            return serialize_int(_pvCtx, _piAddr, _piBuffer, _piBufferSize);
            break;
        default:
            return -1;
            break;
    }
}
