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
#include "Scierror.h"
#include "basic_functions.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "api_oldstack.h"

int triu_matrix(int* _piKey, int* _piAddress, int _iOffset);
/*--------------------------------------------------------------------------*/
int sci_triu(char *fname,int* _piKey)
{
	SciErr sciErr;
	int iRet			= 0;
	int iOffset		= 0;
	int *piAddr1	= NULL;

	CheckRhs(1,2);
	CheckLhs(1,1);


	if(Rhs == 2)
	{//Get offset
		int* piAddr2			= 0;
		double dblReal		= 0;

		sciErr = getVarAddressFromPosition(_piKey, 2, &piAddr2);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		iRet = getScalarDouble(_piKey, piAddr2, &dblReal);
		if(iRet)
		{
			return 1;
		}
		
		iOffset = (int)dblReal;
	}

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(isDoubleType(_piKey, piAddr1))
	{
		iRet = triu_matrix(_piKey, piAddr1, iOffset);
	}
	else if(isPolyType(_piKey, piAddr1))
	{
		//call sci_ptriu
	}
	else
	{
		OverLoad(1);
	}

	if(iRet)
	{
		return 1;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int triu_matrix(int* _piKey, int* _piAddress, int _iOffset)
{
	SciErr sciErr;
	int i;
	int iRet						= 0;
	int iRows						= 0;
	int iCols						= 0;

	double *pdblReal		= NULL;
	double *pdblImg			= NULL;

	double *pdblRealRet	= NULL;
	double *pdblImgRet	= NULL;

	if(isVarComplex(_piKey, _piAddress))
	{
		sciErr = getComplexMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return sciErr.iErr;
		}

		sciErr = allocComplexMatrixOfDouble(_piKey, Rhs + 1, iRows, iCols, &pdblRealRet, &pdblImgRet);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return sciErr.iErr;
		}

		memset(pdblRealRet, 0x00	, sizeof(double) * iRows * iCols);
		memset(pdblImgRet	, 0x00	, sizeof(double) * iRows * iCols);

		for(i = 0 ; i < iCols ; i++)
		{
			int iSize	= Min(Max(i + 1 - _iOffset, 0), iRows);
			memcpy(&pdblRealRet[i * iRows], &pdblReal[i * iRows], sizeof(double) * iSize);
			memcpy(&pdblImgRet[i * iRows]	, &pdblImg[i * iRows]	, sizeof(double) * iSize);
		}
	}
	else
	{
		sciErr = getMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblReal);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return sciErr.iErr;
		}

		sciErr = allocMatrixOfDouble(_piKey, Rhs + 1, iRows, iCols, &pdblRealRet);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return sciErr.iErr;
		}

		memset(pdblRealRet, 0x00, sizeof(double) * iRows * iCols);

		for(i = 0 ; i < iCols ; i++)
		{
			int iSize	= Min(Max(i + 1 - _iOffset, 0), iRows);
			memcpy(&pdblRealRet[i * iRows], &pdblReal[i * iRows], sizeof(double) * iSize);
		}
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
