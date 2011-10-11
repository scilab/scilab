/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2009 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include <mpi.h>
#include "api_scilab.h"
#include "gw_mpi.h"
#include "Scierror.h"
#include "MALLOC.h"


int deserialize_double(void* _pvCtx, int* _piBuffer, int _iBufferSize);
int deserialize_string(void* _pvCtx, int* _piBuffer, int _iBufferSize);
int deserialize_boolean(void* _pvCtx, int* _piBuffer, int _iBufferSize);
int deserialize_int(void* _pvCtx, int* _piBuffer, int _iBufferSize);
int deserialize_sparse(void* _pvCtx, int* _piBuffer, int _iBufferSize, BOOL _bData);

int sci_mpi_recv(char *fname,unsigned long fname_len)
{
    SciErr sciErr;
    int iRet        = 0;
    int* piBuffer   = NULL;
    int iBufferSize = 0;

    int* piAddr1    = NULL;
    int* piAddr2    = NULL;
    double Tag      = 0;
    double Rank       = 0;

	MPI_Status status;
    
    CheckRhs(2,2);
    CheckLhs(1,1);


    //Rank
	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
    
    
    if(getScalarDouble(pvApiCtx, piAddr1, &Rank))
    {
        return 1;
    }

    //Tag    
	sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
    
    
    if(getScalarDouble(pvApiCtx, piAddr2, &Tag))
    {
        return 1;
    }

/*
    iRet = mpi_my_recv(&piBuffer, &iBufferSize);
    if(iRet)
    {
        return 1;
    }
*/

    iRet = MPI_Probe(Rank, Tag, MPI_COMM_WORLD, &status);
    if (iRet != MPI_SUCCESS)
    {
	    Scierror(999, "MPI_Probe failed");
        return 1;
    }

    if (MPI_Get_count(&status, MPI_INT, &iBufferSize) != MPI_SUCCESS)
    {
	    Scierror(999, "MPI_Get_count failed");
        return 1;
    }
    
    piBuffer = (int*)MALLOC(sizeof(int) * iBufferSize);
    if(piBuffer == NULL)
    {
	    Scierror(999, "alloc failed");
        return 1;
    }

    iRet = MPI_Recv(piBuffer, iBufferSize, MPI_INT, Rank, Tag, MPI_COMM_WORLD, &status);
    if (iRet != MPI_SUCCESS)
    {
        Scierror(999, "MPI_Recv failed\n");
        return 1;
    }

    switch(piBuffer[0])
    {
    case sci_matrix :
        iRet = deserialize_double(pvApiCtx, piBuffer, iBufferSize);
        break;
    case sci_strings :
        iRet = deserialize_string(pvApiCtx, piBuffer, iBufferSize);
        break;
    case sci_boolean :
        iRet = deserialize_boolean(pvApiCtx, piBuffer, iBufferSize);
        break;
    case sci_sparse :
        iRet = deserialize_sparse(pvApiCtx, piBuffer, iBufferSize, TRUE);
        break;
    case sci_boolean_sparse :
        iRet = deserialize_sparse(pvApiCtx, piBuffer, iBufferSize, FALSE);
        break;
    case sci_ints :
        iRet = deserialize_int(pvApiCtx, piBuffer, iBufferSize);
        break;
    default :
        return 1;
        break;
    }

    free(piBuffer);
    if(iRet)
    {
        return 1;
    }
  
    LhsVar(1) = 1;
    PutLhsVar();
	return 0;
}

int deserialize_double(void* _pvCtx, int* _piBuffer, int _iBufferSize)
{
    SciErr sciErr;
    int iRows       = _piBuffer[1];
    int iCols       = _piBuffer[2];
    int iComplex    = _piBuffer[3];
    double* pdblR   = NULL;
    double* pdblI   = NULL;
    int iSize       = 0;

    //check size of input buffer
    iSize = 4 + (iRows * iCols * (iComplex +1) * (sizeof(double) / sizeof(int)));
    if(iSize != _iBufferSize)
    {
        printf("Bad buffer size : \n\treceived : %d\n\texpected : %d\n", _iBufferSize, iSize);
        return 1;
    }

    pdblR = (double*)(_piBuffer + 4);
    pdblI = pdblR + iRows * iCols;

    if(iComplex)
    {
        sciErr = createComplexMatrixOfDouble(_pvCtx, 1, iRows, iCols, pdblR, pdblI);
    }
    else
    {
        sciErr = createMatrixOfDouble(_pvCtx, 1, iRows, iCols, pdblR);
    } 
   
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
	
	return 0;
}

int deserialize_string(void* _pvCtx, int* _piBuffer, int _iBufferSize)
{
    SciErr sciErr;
    int i           = 0;
    int iRows       = _piBuffer[1];
    int iCols       = _piBuffer[2];
    int iSize       = 0;
    int* piInLen    = _piBuffer + 4;
    char** pstData  = NULL;
    char* pstInData = (char*)(_piBuffer + 4 + iRows * iCols);

    //check size of input buffer
    for(i = 0 ; i < iRows * iCols ; i++)
    {
        iSize += piInLen[i];
    }
    
    if(iSize % 4)
    {
        iSize = iSize * sizeof(char) / sizeof(int) + 1;
    }
    else
    {
        iSize = iSize * sizeof(char) / sizeof(int);
    }

    iSize += (4 + iRows * iCols);

    if(iSize != _iBufferSize)
    {
        printf("Bad buffer size : \n\treceived : %d\n\texpected : %d\n", _iBufferSize, iSize);
        return 1;
    }
    pstData = (char**)malloc(iRows * iCols * sizeof(char*));
        
    for(i = 0 ; i < iRows * iCols ; i++)
    {
        pstData[i] = (char*)malloc((piInLen[i] + 1) * sizeof(char));
        memcpy(pstData[i], pstInData, piInLen[i]);
        pstData[i][piInLen[i]] = 0;
        pstInData += piInLen[i];
    }
    
    sciErr = createMatrixOfString(_pvCtx, 1, iRows, iCols, (const char* const*)pstData);
    for(i = 0 ; i < iRows * iCols ; i++)
    {
        free(pstData[i]);
    }
    free(pstData);

	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}

    return 0;
}

int deserialize_boolean(void* _pvCtx, int* _piBuffer, int _iBufferSize)
{
    SciErr sciErr;
    int iRows       = _piBuffer[1];
    int iCols       = _piBuffer[2];
    int iSize       = 0;
    int* piBool     = NULL;

    //check size of input buffer
    iSize = 4 + (iRows * iCols);
    if(iSize != _iBufferSize)
    {
        printf("Bad buffer size : \n\treceived : %d\n\texpected : %d\n", _iBufferSize, iSize);
        return 1;
    }
    
    piBool = _piBuffer + 4;
    sciErr = createMatrixOfBoolean(_pvCtx, 1, iRows, iCols, piBool);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
    
    return 0;
}

int deserialize_int(void* _pvCtx, int* _piBuffer, int _iBufferSize)
{
    SciErr sciErr;
    int iRows       = _piBuffer[1];
    int iCols       = _piBuffer[2];
    int iPrecision  = _piBuffer[3];
    int iItemSize   = 0;
    int iSize       = 0;
    void* pvData    = NULL;

    //check size of input buffer
    if(iPrecision == SCI_INT8 || iPrecision == SCI_UINT8)
    {
        iItemSize = sizeof(char);
    }
    else if(iPrecision == SCI_INT16 || iPrecision == SCI_UINT16)
    {
        iItemSize = sizeof(short);
    }
    else if(iPrecision == SCI_INT32 || iPrecision == SCI_UINT32)
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
    if((iSize * iItemSize) % sizeof(int))
    {
        iSize = (iSize * iItemSize) / sizeof(int) + 1;
    }
    else
    {
        iSize = (iSize * iItemSize) / (sizeof(int));
    }

    iSize += 4;
    if(iSize != _iBufferSize)
    {
        printf("Bad buffer size : \n\treceived : %d\n\texpected : %d\n", _iBufferSize, iSize);
        return 1;
    }
    
    pvData = _piBuffer + 4;

    switch(iPrecision)
    {
    case SCI_INT8 : 
        sciErr = createMatrixOfInteger8(_pvCtx, 1, iRows, iCols, (char*)pvData);
        break;
    case SCI_UINT8 : 
        sciErr = createMatrixOfUnsignedInteger8(_pvCtx, 1, iRows, iCols, (unsigned char*)pvData);
        break;
    case SCI_INT16 : 
        sciErr = createMatrixOfInteger16(_pvCtx, 1, iRows, iCols, (short*)pvData);
        break;
    case SCI_UINT16 : 
        sciErr = createMatrixOfUnsignedInteger16(_pvCtx, 1, iRows, iCols, (unsigned short*)pvData);
        break;
    case SCI_INT32 : 
        sciErr = createMatrixOfInteger32(_pvCtx, 1, iRows, iCols, (int*)pvData);
        break;
    case SCI_UINT32 : 
        sciErr = createMatrixOfUnsignedInteger32(_pvCtx, 1, iRows, iCols, (unsigned int*)pvData);
        break;
/*
    case SCI_INT64 : 
        sciErr = createMatrixOfInteger64(_pvCtx, 1, iRows, iCols, (long long*)pvData);
        break;
    case SCI_UINT64 : 
        sciErr = createMatrixOfUnsignedInteger64(_pvCtx, 1, iRows, iCols, (unsigned long long*)pvData);
        break;
*/
    default :
        break;
    }

	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
    return 0;
}

int deserialize_sparse(void* _pvCtx, int* _piBuffer, int _iBufferSize, BOOL _bData)
{
    SciErr sciErr;
    int iRows       = _piBuffer[1];
    int iCols       = _piBuffer[2];
    int iComplex    = _piBuffer[3];
    int iItemCount  = _piBuffer[4];
    int* piRowCount = NULL;
    int* piColPos   = NULL;
    double* pdblR   = NULL;
    double* pdblI   = NULL;
    int iSize       = 0;
    void* pvData    = NULL;

    iSize = 5 + iRows + iItemCount;
    if(_bData)
    {
        iSize += iItemCount * (iComplex+1) * sizeof(double) / sizeof(int);
    }

    if(iSize != _iBufferSize)
    {
        printf("Bad buffer size : \n\treceived : %d\n\texpected : %d\n", _iBufferSize, iSize);
        return 1;
    }
    
    piRowCount  = _piBuffer + 5;
    piColPos    = _piBuffer + 5 + iRows;
    
    if(_bData)
    {
        pdblR = (double*)(_piBuffer + 5 + iRows + iItemCount);
        if(iComplex)
        {
            pdblI = pdblR + iItemCount;
            sciErr = createComplexSparseMatrix(_pvCtx, 1, iRows, iCols, iItemCount, piRowCount, piColPos, pdblR, pdblI);
        }
        else
        {
            sciErr = createSparseMatrix(_pvCtx, 1, iRows, iCols, iItemCount, piRowCount, piColPos, pdblR);
        }
    }
    else
    {
        sciErr = createBooleanSparseMatrix(_pvCtx, 1, iRows, iCols, iItemCount, piRowCount, piColPos);
    }
    
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 1;
	}
    return 0;
}

