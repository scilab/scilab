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
#include <stdlib.h>
#include "api_scilab.h"
#include "BOOL.h"
#include "sci_malloc.h"
#include "deserialization.h"

static int deserialize_double(void *_pvCtx, int *_piBuffer, int _iBufferSize)
{
    SciErr sciErr;
    int iRows = _piBuffer[1];
    int iCols = _piBuffer[2];
    int iComplex = _piBuffer[3];
    double *pdblR = NULL;
    double *pdblI = NULL;
    int iSize = 0;

    //check size of input buffer
    iSize = 4 + (iRows * iCols * (iComplex + 1) * (sizeof(double) / sizeof(int)));
    if (iSize != _iBufferSize)
    {
        printf("Bad buffer size: \n\tReceived: %d\n\tExpected: %d\n", _iBufferSize, iSize);
        return 1;
    }

    pdblR = (double *)(_piBuffer + 4);
    pdblI = pdblR + iRows * iCols;

    if (iComplex)
    {
        sciErr = createComplexMatrixOfDouble(_pvCtx, nbInputArgument(_pvCtx) + 1, iRows, iCols, pdblR, pdblI);
    }
    else
    {
        sciErr = createMatrixOfDouble(_pvCtx, nbInputArgument(_pvCtx) + 1, iRows, iCols, pdblR);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    return 0;
}

static int deserialize_string(void *_pvCtx, int *_piBuffer, int _iBufferSize)
{
    SciErr sciErr;
    int i = 0;
    int iRows = _piBuffer[1];
    int iCols = _piBuffer[2];
    int iSize = 0;
    int *piInLen = _piBuffer + 4;
    char **pstData = NULL;
    char *pstInData = (char *)(_piBuffer + 4 + iRows * iCols);

    //check size of input buffer
    for (i = 0; i < iRows * iCols; i++)
    {
        iSize += piInLen[i];
    }

    if (iSize % 4)
    {
        iSize = iSize * sizeof(char) / sizeof(int) + 1;
    }
    else
    {
        iSize = iSize * sizeof(char) / sizeof(int);
    }

    iSize += (4 + iRows * iCols);

    if (iSize != _iBufferSize)
    {
        printf("Bad buffer size: \n\tReceived: %d\n\tExpected: %d\n", _iBufferSize, iSize);
        return 1;
    }
    pstData = (char **)MALLOC(iRows * iCols * sizeof(char *));

    for (i = 0; i < iRows * iCols; i++)
    {
        pstData[i] = (char *)MALLOC((piInLen[i] + 1) * sizeof(char));
        memcpy(pstData[i], pstInData, piInLen[i]);
        pstData[i][piInLen[i]] = 0;
        pstInData += piInLen[i];
    }

    sciErr = createMatrixOfString(_pvCtx, nbInputArgument(_pvCtx) + 1, iRows, iCols, (const char * const *)pstData);
    for (i = 0; i < iRows * iCols; i++)
    {
        FREE(pstData[i]);
    }

    FREE(pstData);

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    return 0;
}

static int deserialize_boolean(void *_pvCtx, int *_piBuffer, int _iBufferSize)
{
    SciErr sciErr;
    int iRows = _piBuffer[1];
    int iCols = _piBuffer[2];
    int iSize = 0;
    int *piBool = NULL;

    //check size of input buffer
    iSize = 4 + (iRows * iCols);
    if (iSize != _iBufferSize)
    {
        printf("Bad buffer size: \n\tReceived: %d\n\tExpected: %d\n", _iBufferSize, iSize);
        return 1;
    }

    piBool = _piBuffer + 4;
    sciErr = createMatrixOfBoolean(_pvCtx, nbInputArgument(_pvCtx) + 1, iRows, iCols, piBool);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    return 0;
}

static int deserialize_int(void *_pvCtx, int *_piBuffer, int _iBufferSize)
{
    SciErr sciErr;
    int iRows = _piBuffer[1];
    int iCols = _piBuffer[2];
    int iPrecision = _piBuffer[3];
    int iItemSize = 0;
    int iSize = 0;
    void *pvData = NULL;

    //check size of input buffer
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
    iSize = iRows * iCols;
    if ((iSize * iItemSize) % sizeof(int))
    {
        iSize = (iSize * iItemSize) / sizeof(int) + 1;
    }
    else
    {
        iSize = (iSize * iItemSize) / (sizeof(int));
    }

    iSize += 4;
    if (iSize != _iBufferSize)
    {
        printf("Bad buffer size: \n\tReceived: %d\n\tExpected: %d\n", _iBufferSize, iSize);
        return 1;
    }

    pvData = _piBuffer + 4;

    switch (iPrecision)
    {
        case SCI_INT8:
            sciErr = createMatrixOfInteger8(_pvCtx, nbInputArgument(_pvCtx) + 1, iRows, iCols, (char *)pvData);
            break;
        case SCI_UINT8:
            sciErr = createMatrixOfUnsignedInteger8(_pvCtx, nbInputArgument(_pvCtx) + 1, iRows, iCols, (unsigned char *)pvData);
            break;
        case SCI_INT16:
            sciErr = createMatrixOfInteger16(_pvCtx, nbInputArgument(_pvCtx) + 1, iRows, iCols, (short *)pvData);
            break;
        case SCI_UINT16:
            sciErr = createMatrixOfUnsignedInteger16(_pvCtx, nbInputArgument(_pvCtx) + 1, iRows, iCols, (unsigned short *)pvData);
            break;
        case SCI_INT32:
            sciErr = createMatrixOfInteger32(_pvCtx, nbInputArgument(_pvCtx) + 1, iRows, iCols, (int *)pvData);
            break;
        case SCI_UINT32:
            sciErr = createMatrixOfUnsignedInteger32(_pvCtx, nbInputArgument(_pvCtx) + 1, iRows, iCols, (unsigned int *)pvData);
            break;
        /*
            case SCI_INT64 :
                sciErr = createMatrixOfInteger64(_pvCtx, nbInputArgument(_pvCtx) + 1, iRows, iCols, (long long*)pvData);
                break;
            case SCI_UINT64 :
                sciErr = createMatrixOfUnsignedInteger64(_pvCtx, nbInputArgument(_pvCtx) + 1, iRows, iCols, (unsigned long long*)pvData);
                break;
        */
        default:
            break;
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }
    return 0;
}

static int deserialize_sparse(void *_pvCtx, int *_piBuffer, int _iBufferSize, BOOL _bData)
{
    SciErr sciErr;
    int iRows = _piBuffer[1];
    int iCols = _piBuffer[2];
    int iComplex = _piBuffer[3];
    int iItemCount = _piBuffer[4];
    int *piRowCount = NULL;
    int *piColPos = NULL;
    double *pdblR = NULL;
    double *pdblI = NULL;
    int iSize = 0;

    iSize = 5 + iRows + iItemCount;
    if (_bData)
    {
        iSize += iItemCount * (iComplex + 1) * sizeof(double) / sizeof(int);
    }

    if (iSize != _iBufferSize)
    {
        printf("Bad buffer size: \n\tReceived: %d\n\tExpected: %d\n", _iBufferSize, iSize);
        return 1;
    }

    piRowCount = _piBuffer + 5;
    piColPos = _piBuffer + 5 + iRows;

    if (_bData)
    {
        pdblR = (double *)(_piBuffer + 5 + iRows + iItemCount);
        if (iComplex)
        {
            pdblI = pdblR + iItemCount;
            sciErr = createComplexSparseMatrix(_pvCtx, nbInputArgument(_pvCtx) + 1, iRows, iCols, iItemCount, piRowCount, piColPos, pdblR, pdblI);
        }
        else
        {
            sciErr = createSparseMatrix(_pvCtx, nbInputArgument(_pvCtx) + 1, iRows, iCols, iItemCount, piRowCount, piColPos, pdblR);
        }
    }
    else
    {
        sciErr = createBooleanSparseMatrix(_pvCtx, nbInputArgument(_pvCtx) + 1, iRows, iCols, iItemCount, piRowCount, piColPos);
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }
    return 0;
}

int deserialize_from_mpi(void *_pvCtx, int *_piBuffer, int _iBufferSize)
{
    switch (*_piBuffer)
    {
        case sci_matrix:
            return deserialize_double(_pvCtx, _piBuffer, _iBufferSize);
        case sci_strings:
            return deserialize_string(_pvCtx, _piBuffer, _iBufferSize);
        case sci_boolean:
            return deserialize_boolean(_pvCtx, _piBuffer, _iBufferSize);
        case sci_sparse:
            return deserialize_sparse(_pvCtx, _piBuffer, _iBufferSize, TRUE);
        case sci_boolean_sparse:
            return deserialize_sparse(_pvCtx, _piBuffer, _iBufferSize, FALSE);
        case sci_ints:
            return deserialize_int(_pvCtx, _piBuffer, _iBufferSize);
        default:
            return -1; //unknown type
    }
}


