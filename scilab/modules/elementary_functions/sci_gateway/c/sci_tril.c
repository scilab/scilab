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

/*--------------------------------------------------------------------------*/
int tril_matrix(int* _piAddress, int _iOffset, int* _piKey);

/*--------------------------------------------------------------------------*/
int sci_tril(char *fname, int* _piKey)
{
	int iRet			= 0;
	int iOffset		= 0;
	int *piAddr1	= NULL;

	CheckRhs(1,2);
	CheckLhs(1,1);


	if(Rhs == 2)
	{//Get offset
		int iRows2				= 0;
		int iCols2				= 0;
		int* piAddr2			= 0;
		double* pdblReal	= NULL;

		iRet = getVarAddressFromPosition(2, &piAddr2, _piKey);
		if(iRet)
		{
			return 1;
		}

		if(getVarType(piAddr2) != sci_matrix)
		{
			Error(53);
			return 0;
		}

		if(isVarComplex(piAddr2))
		{
			Error(52);
			return 0;
		}

		iRet = getVarDimension(piAddr2, &iRows2, &iCols2);
		if(iRet || iRows2 != 1 || iCols2 != 1)
		{
			Error(89);
			return 0;
		}

		getMatrixOfDouble(piAddr2, &iRows2, &iCols2, &pdblReal);
		if(iRet)
		{
			return 1;
		}
		iOffset = (int)pdblReal[0];
	}

	iRet = getVarAddressFromPosition(1, &piAddr1, _piKey);
	if(iRet)
	{
		return 1;
	}

	switch(getVarType(piAddr1))
	{
	case sci_matrix :
		iRet = tril_matrix(piAddr1, iOffset, _piKey);
		break;
	case sci_poly :
		//call sci_ptril
		break;
	default : 
		OverLoad(1);
		break;
	}

	if(iRet)
	{
		return 1;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int tril_matrix(int* _piAddress, int _iOffset, int* _piKey)
{
	int i;
	int iRet						= 0;
	int iRows						= 0;
	int iCols						= 0;

	double *pdblReal		= NULL;
	double *pdblImg			= NULL;

	double *pdblRealRet	= NULL;
	double *pdblImgRet	= NULL;

	if(isVarComplex(_piAddress))
	{
		iRet = getComplexMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
		if(iRet)
		{
			return 1;
		}

		iRet = allocComplexMatrixOfDouble(Rhs + 1, iRows, iCols, &pdblRealRet, &pdblImgRet, _piKey);
		if(iRet)
		{
			return 1;
		}

		memcpy(pdblRealRet, pdblReal, sizeof(double) * iRows * iCols);
		memcpy(pdblImgRet	, pdblImg	, sizeof(double) * iRows * iCols);

		for(i = 0 ; i < iCols ; i++)
		{
			int iSize	= Min(Max(i - _iOffset, 0), iRows);
			memset(&pdblRealRet[i * iRows], 0x00, sizeof(double) * iSize);
			memset(&pdblImgRet[i * iRows],	0x00, sizeof(double) * iSize);
		}
	}
	else
	{
		iRet = getMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblReal);
		if(iRet)
		{
			return 1;
		}

		iRet = allocMatrixOfDouble(Rhs + 1, iRows, iCols, &pdblRealRet, _piKey);
		if(iRet)
		{
			return 1;
		}

		memcpy(pdblRealRet, pdblReal, sizeof(double) * iRows * iCols);

		for(i = 0 ; i < iCols ; i++)
		{
			int iSize	= Min(Max(i - _iOffset, 0), iRows);
			memset(&pdblRealRet[i * iRows], 0x00, sizeof(double) * iSize);
		}
	}

	return 0;
}


/*--------------------------------------------------------------------------*/
