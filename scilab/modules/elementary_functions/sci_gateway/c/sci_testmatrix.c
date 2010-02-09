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
#include "gw_elementary_functions.h"
#include "stack-c.h"
#include "basic_functions.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "api_oldstack.h"

#define MAGI_LETTER		'm'
#define FRK_LETTER		'f'
#define HILB_LETTER		'h'

char getGenerateMode(int* _piKey, int* _piAddress);
int getGenerateSize(int* _piKey, int* _piAddress);
/*--------------------------------------------------------------------------*/
int sci_testmatrix(char *fname, int* _piKey)
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
	
	CheckRhs(2,2);
	CheckLhs(1,1);

	/*check input 1*/
	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarAddressFromPosition(_piKey, 2, &piAddr2);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	cMode = getGenerateMode(_piKey, piAddr1);

	if(cMode == -1)
	{
		return 1;
	}

	iDim = getGenerateSize(_piKey, piAddr2);


	if(cMode != FRK_LETTER && cMode != HILB_LETTER && iDim == 2)
	{
		iDim = 0;
	}

	if(iDim == 0)
	{
		iRet = createEmptyMatrix(_piKey, Rhs + 1);
		if(iRet)
		{
			return 1;
		}
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		return 0;
	}

	sciErr = allocMatrixOfDouble(_piKey, Rhs + 1, iDim, iDim, &pdblRealRet);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	switch(cMode)
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

char getGenerateMode(int* _piKey, int* _piAddress)
{
	int iRet = 0;

	char* pstData;

	iRet = getAllocatedSingleString(_piKey, _piAddress, &pstData);
	if(iRet)
	{
		return -1;
	}
	
	return pstData[0];
}

int getGenerateSize(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	int iRet = 0;
	int iRows = 0;
	int iCols = 0;

	double* pdblReal = NULL;
	double* pdblImg	 = NULL;

	if(isVarComplex(_piKey, _piAddress))
	{
		sciErr = getComplexMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}
	else
	{
		sciErr = getMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblReal);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}
	return abs((int)pdblReal[0]);
	
}
/*--------------------------------------------------------------------------*/
