/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Jean-Baptiste SILVY
* Copyright (C) 2007 - INRIA - Allan CORNET
* Copyright (C) 2007 - INRIA - Cong WU
* Copyright (C) 2009 - INRIA - Antoine ELIAS
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_elementary_functions.h"
#include "stack-c.h"
#include "gsort.h"
#include "string.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "isanan.h"
#include "api_scilab.h"

const char g_pcstProcessGlobal[] = "g";
const char g_pcstProcessRow[] = "r";
const char g_pcstProcessCol[] = "c";
const char g_pcstProcessListRow[] = "lr";
const char g_pcstProcessListCol[] = "lc";

const char g_pcstWayDec[] = "d";
const char g_pcstWayInc[] = "i";
/* Next define intruction put here instead of stack-c.h waiting for
more genral usage stack-c.h macros CreateVarFromPtr and CreateVar
contains a "return 0" which make impossible to free indices in case
of error
*/
#define CreateVarFromPtrNoCheck(n,ct,mx,nx,lx) C2F(createvarfromptr)((c_local=n,&c_local),ct,mx,nx,(void *)lx,1L)
#define CreateVarNoCheck(n,ct,mx,nx,lx) C2F(createvar)((c_local=n,&c_local),ct,mx,nx,(void *)lx, 1L)

int checkInputValue(char* _pstInput, const char** _pcstRef, int _iRefCount);
char* getWayType(void* pvApiCtx, int* _piAddress);
char* getProcessType(void* pvApiCtx, int* _piAddress);
int computeIndiceDim(const char* _pcstProcess, int _iRowsIn, int _iColsIn,  int* _piRowsOut, int* _piColsOut);

int	gsort_int(void* pvApiCtx, int * _piAddress, const char* _pcstProcess, const char* _pcstWay);
int	gsort_string(void* pvApiCtx, int * _piAddress, const char* _pcstProcess, const char* _pcstWay);
int	gsort_double(void* pvApiCtx, int * _piAddress, const char* _pcstProcess, const char* _pcstWay);
/*--------------------------------------------------------------------------*/
static int gsort_complex(char *fname, char *mode, char *order);
/*--------------------------------------------------------------------------*/
int sci_gsort(char *fname, void* pvApiCtx)
{
	SciErr sciErr;
	int iRet								= 0;

	int iRows1							= 0;
	int iCols1							= 0;
	int iType1							= 0;

	int* piAddr1						= NULL;
	int* piAddr2						= NULL;
	int* piAddr3						= NULL;

	const char* pcstProcess	= g_pcstProcessGlobal;
	const char* pcstWay			= g_pcstWayDec;

	CheckRhs(1,3);
	CheckLhs(1,2);

	if(Rhs > 1)
	{//get option
		sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		pcstProcess = getProcessType(pvApiCtx, piAddr2); 
		if(pcstProcess == NULL)
		{
			return 1;
		}
	}

	if(Rhs == 3)
	{//get way ( inc or dec )
		sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		pcstWay = getWayType(pvApiCtx, piAddr3); 
		if(pcstWay == NULL)
		{
			return 1;
		}
	}

	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	
	sciErr = getVarType(pvApiCtx, piAddr1, &iType1);
	switch(iType1)
	{
	case sci_matrix : 
		iRet = gsort_double(pvApiCtx, piAddr1, pcstProcess, pcstWay);
		break;
	case sci_strings : 
		iRet = gsort_string(pvApiCtx, piAddr1, pcstProcess, pcstWay);
		break;
	case sci_ints : 
		iRet = gsort_int(pvApiCtx, piAddr1, pcstProcess, pcstWay);
		break;
	default :
		return 1;
	}

	if(iRet)
	{
		return 1;
	}

	LhsVar(1) = Rhs + 1;

	if(Lhs == 2)
	{
		LhsVar(2) = Rhs + 2;
	}

	PutLhsVar();
	return 0;
} 


int	gsort_double(void* pvApiCtx, int * _piAddress, const char* _pcstProcess, const char* _pcstWay)
{
	SciErr sciErr;
	int iRet							= 0;
	int iRows							= 0;
	int iCols							= 0;
	int iRowsInd					= 0;
	int iColsInd					= 0;
	double* pdblReal			= NULL;
	double* pdblRealOut1	= NULL;
	double* pdblRealOut2	= NULL;
	int *piInd						= NULL;
	int iLhs2							= Lhs == 2;

	if(isEmptyMatrix(pvApiCtx, _piAddress))
	{
		iRet = createEmptyMatrix(pvApiCtx, Rhs + 1);
		if(iRet)
		{
			return iRet;
		}

		if(Lhs == 2)
		{		
			iRet = createEmptyMatrix(pvApiCtx, Rhs + 1);
			if(iRet)
			{
				return iRet;
			}
		}		
	}
	
	if(isVarComplex(pvApiCtx, _piAddress))
	{
        char fname[] = "gsort";
		OverLoad(1); //call %_gsort.sci
		return 0;
	}
	
	sciErr = getMatrixOfDouble(pvApiCtx, _piAddress, &iRows, &iCols, &pdblReal);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealOut1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	memcpy(pdblRealOut1, pdblReal, sizeof(double) * iRows * iCols);
	iRet = computeIndiceDim(_pcstProcess, iRows, iCols, &iRowsInd, &iColsInd);
	if(iRet)
	{
		return iRet;
	}

	piInd = (int*)MALLOC(sizeof(int) * iRowsInd * iColsInd);
	C2F(gsortd)(pdblRealOut1, piInd , &iLhs2, &iRows, &iCols, (char*)_pcstProcess, (char*)_pcstWay);

	if(Lhs == 2)
	{
		iRet = createMatrixOfDoubleFromInteger(pvApiCtx, Rhs + 2, iRowsInd, iColsInd, piInd);
		if(iRet)
		{
			FREE(piInd);
			return iRet;
		}
	}

	FREE(piInd);
	return 0;
}

int	gsort_string(void* pvApiCtx, int * _piAddress, const char* _pcstProcess, const char* _pcstWay)
{
	SciErr sciErr;
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int iRowsInd		= 0;
	int iColsInd		= 0;
	int iLhs2				= Lhs == 2;

	int* piInd			= NULL;
	char** pstData	= NULL;

	iRet = getAllocatedMatrixOfString(pvApiCtx, _piAddress, &iRows, &iCols, &pstData);
	if(iRet)
	{
		freeAllocatedMatrixOfString(iRows, iCols, pstData);
		return iRet;
	}

	iRet = computeIndiceDim(_pcstProcess, iRows, iCols, &iRowsInd, &iColsInd);
	if(iRet)
	{
		freeAllocatedMatrixOfString(iRows, iCols, pstData);
		return iRet;
	}

	piInd = (int*)MALLOC(sizeof(int) * iRowsInd * iColsInd);
	C2F(gsorts)(pstData, piInd, &iLhs2, &iRows,&iCols, (char*)_pcstProcess, (char*)_pcstWay);
	
	sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, iRows, iCols, pstData);
	if(sciErr.iErr)
	{
		freeAllocatedMatrixOfString(iRows, iCols, pstData);
		FREE(piInd);
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	if(Lhs == 2)
	{
		iRet = createMatrixOfDoubleFromInteger(pvApiCtx, Rhs + 2, iRowsInd, iColsInd, piInd);
		if(iRet)
		{
			FREE(piInd);
			return iRet;
		}
	}
	
	FREE(piInd);
	return 0;
}

int	gsort_int(void* pvApiCtx, int * _piAddress, const char* _pcstProcess, const char* _pcstWay)
{
	SciErr sciErr;
	int iRet			= 0;
	int iRows			= 0;
	int iCols			= 0;
	int iRowsInd	= 0;
	int iColsInd	= 0;
	int iPrec			= 0;
	int iLhs2			= Lhs == 2;

	int* piInd		= NULL;

	sciErr = getVarDimension(pvApiCtx, _piAddress, &iRows, &iCols);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	iRet = computeIndiceDim(_pcstProcess, iRows, iCols, &iRowsInd, &iColsInd);
	if(iRet)
	{
		return iRet;
	}

	piInd = (int*)MALLOC(sizeof(int) * iRowsInd * iColsInd);

	sciErr = getMatrixOfIntegerPrecision(pvApiCtx, _piAddress, &iPrec);
	if(sciErr.iErr)
	{
		FREE(piInd);
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	switch(iPrec)
	{
	case SCI_INT8 : 
		{
			char* pcDataIn		= NULL;
			char* pcDataOut		= NULL;
			
			sciErr = getMatrixOfInteger8(pvApiCtx, _piAddress, &iRows, &iCols, &pcDataIn);
			if(sciErr.iErr)
			{
				FREE(piInd);
				printError(&sciErr, 0);
				return sciErr.iErr;
			}

			sciErr  = allocMatrixOfInteger8(pvApiCtx, Rhs + 1, iRows, iCols, &pcDataOut);
			if(sciErr.iErr)
			{
				FREE(piInd);
				printError(&sciErr, 0);
				return sciErr.iErr;
			}

			memcpy(pcDataOut, pcDataIn, sizeof(char) * iRows * iCols);
			C2F(gsortchar)(pcDataOut, piInd, &iLhs2, &iRows, &iCols, (char*)_pcstProcess, (char*)_pcstWay);
			break;
		}
	case SCI_UINT8 : 
		{
			unsigned char* pucDataIn		= NULL;
			unsigned char* pucDataOut		= NULL;
			
			sciErr = getMatrixOfUnsignedInteger8(pvApiCtx, _piAddress, &iRows, &iCols, &pucDataIn);
			if(sciErr.iErr)
			{
				FREE(piInd);
				printError(&sciErr, 0);
				return sciErr.iErr;
			}

			sciErr = allocMatrixOfUnsignedInteger8(pvApiCtx, Rhs + 1, iRows, iCols, &pucDataOut);
			if(sciErr.iErr)
			{
				FREE(piInd);
				printError(&sciErr, 0);
				return sciErr.iErr;
			}

			memcpy(pucDataOut, pucDataIn, sizeof(unsigned char) * iRows * iCols);
			C2F(gsortuchar)(pucDataOut, piInd, &iLhs2, &iRows, &iCols, (char*)_pcstProcess, (char*)_pcstWay);
			break;
		}
	case SCI_INT16 : 
		{
			short* psDataIn		= NULL;
			short* psDataOut	= NULL;
			
			sciErr = getMatrixOfInteger16(pvApiCtx, _piAddress, &iRows, &iCols, &psDataIn);
			if(sciErr.iErr)
			{
				FREE(piInd);
				printError(&sciErr, 0);
				return sciErr.iErr;
			}

			sciErr = allocMatrixOfInteger16(pvApiCtx, Rhs + 1, iRows, iCols, &psDataOut);
			if(sciErr.iErr)
			{
				FREE(piInd);
				printError(&sciErr, 0);
				return sciErr.iErr;
			}

			memcpy(psDataOut, psDataIn, sizeof(short) * iRows * iCols);
			C2F(gsortshort)(psDataOut, piInd, &iLhs2, &iRows, &iCols, (char*)_pcstProcess, (char*)_pcstWay);
			break;
		}
	case SCI_UINT16 : 
		{
			unsigned short* pusDataIn		= NULL;
			unsigned short* pusDataOut	= NULL;
			
			sciErr = getMatrixOfUnsignedInteger16(pvApiCtx, _piAddress, &iRows, &iCols, &pusDataIn);
			if(sciErr.iErr)
			{
				FREE(piInd);
				printError(&sciErr, 0);
				return sciErr.iErr;
			}

			sciErr = allocMatrixOfUnsignedInteger16(pvApiCtx, Rhs + 1, iRows, iCols, &pusDataOut);
			if(sciErr.iErr)
			{
				FREE(piInd);
				printError(&sciErr, 0);
				return sciErr.iErr;
			}

			memcpy(pusDataOut, pusDataIn, sizeof(unsigned short) * iRows * iCols);
			C2F(gsortushort)(pusDataOut, piInd, &iLhs2, &iRows, &iCols, (char*)_pcstProcess, (char*)_pcstWay);
			break;
		}
	case SCI_INT32 : 
		{
			int* piDataIn		= NULL;
			int* piDataOut	= NULL;
			
			sciErr = getMatrixOfInteger32(pvApiCtx, _piAddress, &iRows, &iCols, &piDataIn);
			if(sciErr.iErr)
			{
				FREE(piInd);
				printError(&sciErr, 0);
				return sciErr.iErr;
			}

			sciErr = allocMatrixOfInteger32(pvApiCtx, Rhs + 1, iRows, iCols, &piDataOut);
			if(sciErr.iErr)
			{
				FREE(piInd);
				printError(&sciErr, 0);
				return sciErr.iErr;
			}

			memcpy(piDataOut, piDataIn, sizeof(int) * iRows * iCols);
			C2F(gsortint)(piDataOut, piInd, &iLhs2, &iRows, &iCols, (char*)_pcstProcess, (char*)_pcstWay);
			break;
		}
	case SCI_UINT32 : 
		{
			unsigned int* puiDataIn		= NULL;
			unsigned int* puiDataOut	= NULL;
			
			sciErr = getMatrixOfUnsignedInteger32(pvApiCtx, _piAddress, &iRows, &iCols, &puiDataIn);
			if(sciErr.iErr)
			{
				FREE(piInd);
				printError(&sciErr, 0);
				return sciErr.iErr;
			}

			sciErr = allocMatrixOfUnsignedInteger32(pvApiCtx, Rhs + 1, iRows, iCols, &puiDataOut);
			if(sciErr.iErr)
			{
				FREE(piInd);
				printError(&sciErr, 0);
				return sciErr.iErr;
			}

			memcpy(puiDataOut, puiDataIn, sizeof(unsigned int) * iRows * iCols);
			C2F(gsortuint)(puiDataOut, piInd, &iLhs2, &iRows, &iCols, (char*)_pcstProcess, (char*)_pcstWay);
			break;
		}
	default : 
		FREE(piInd);
		return 1;
	}

	if(Lhs == 2)
	{
		iRet = createMatrixOfDoubleFromInteger(pvApiCtx, Rhs + 2, iRowsInd, iColsInd, piInd);
		if(iRet)
		{
			FREE(piInd);
			return iRet;
		}
	}

	FREE(piInd);
	return 0;
}

static int gsort_complex(char *fname, char *mode, char *order)
{
	int lw = 0;
	C2F(overload)(&lw, "gsort", (unsigned long)strlen("gsort"));
	return 0;
}
/*-----------------------------------------------------------------------------------*/

char* getWayType(void* pvApiCtx, int* _piAddress)
{
	int iRet					= 0;
	int iRows					= 0;
	int iCols					= 0;
	char* pstWay 			= NULL;

	const char* pcstRef[2] = {g_pcstWayDec,g_pcstWayInc};
	
	iRet = getAllocatedSingleString(pvApiCtx, _piAddress, &pstWay);
	if(iRet)
	{
		freeAllocatedSingleString(pstWay);
		return NULL;
	}

	iRet = checkInputValue(pstWay, pcstRef, 2);
	if(iRet)
	{
		freeAllocatedSingleString(pstWay);
		return NULL;
	}
	return pstWay;
}

char* getProcessType(void* pvApiCtx, int* _piAddress)
{
	int iRet					= 0;
	int iRows					= 0;
	int iCols					= 0;
	char* pstProcess	= NULL;

	const char* pcstRef[5] = {g_pcstProcessGlobal,g_pcstProcessRow,g_pcstProcessCol,g_pcstProcessListRow,g_pcstProcessListCol};
	
	iRet = getAllocatedSingleString(pvApiCtx, _piAddress, &pstProcess);
	if(iRet)
	{
		freeAllocatedSingleString(pstProcess);
		return NULL;
	}

	iRet = checkInputValue(pstProcess, pcstRef, 5);
	if(iRet)
	{
		freeAllocatedSingleString(pstProcess);
		return NULL;
	}
	return pstProcess;
}

int checkInputValue(char* _pstInput, const char** _pcstRef, int _iRefCount)
{
	int i;
	for(i = 0 ; i < _iRefCount ; i++)
	{
		if(strcmp(_pstInput, _pcstRef[i]) == 0)
		{//find
			return 0;
		}
	}
	//not find
	return 1;
}

int computeIndiceDim(const char* _pcstProcess, int _iRowsIn, int _iColsIn,  int* _piRowsOut, int* _piColsOut)
{
	if(strcmp(_pcstProcess, g_pcstProcessListRow) == 0)
	{
		*_piRowsOut = _iRowsIn;
		*_piColsOut = 1;
	}
	else if(strcmp(_pcstProcess, g_pcstProcessListCol) == 0)
	{
		*_piRowsOut = 1;
		*_piColsOut = _iColsIn;
	}
	else
	{
		*_piRowsOut = _iRowsIn;
		*_piColsOut = _iColsIn;
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
