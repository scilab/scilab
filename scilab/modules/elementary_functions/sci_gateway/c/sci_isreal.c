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
#include "api_scilab.h"
#include "Scierror.h"

int isreal_double(int* _piAddress, double _dblRef, int* _piIsReal);
int isreal_poly(int* _piAddress, double _dblRef, int* _piIsReal);
int isreal_sparse(int* _piAddress, double _dblRef, int* _piIsReal);
int isreal_common(double *_pdblData, int _iSize, double _dblRef, int* _piIsReal);

/*--------------------------------------------------------------------------*/
int C2F(sci_isreal) (char *fname,unsigned long fname_len)
{
	int iRet					= 0;
	int iRows					= 0;
	int iCols					= 0;
	int iType					= 0;

	int* piAddr1			= NULL;
	int* piAddr2			= NULL;
	int* piBool				= NULL;

	double *pdblPrec	= NULL;

	CheckRhs(1,2);
	CheckLhs(1,1);

	iRet = getVarAddressFromPosition(1, &piAddr1);
	if(iRet)
	{
		return 1;
	}

	iType = getVarType(piAddr1);
	if(iType != sci_matrix && iType != sci_poly && iType != sci_sparse)
	{
		OverLoad(1);
		return 0;
	}

	iRet = allocMatrixOfBoolean(Rhs + 1, 1, 1, &piBool);
	if(iRet)
	{
		return 1;
	}

	if(Rhs == 1)
	{
		if(isVarComplex(piAddr1))
			piBool[0] = 0;
		else
			piBool[0] = 1;
	}
	else //Rhs == 2
	{
		double dblPrec = 0;
		iRet = getVarAddressFromPosition(2, &piAddr2);
		if(iRet)
		{
			return 1;
		}

		if(getVarType(piAddr2) != sci_matrix)
		{
			Error(53);
			return 0;
		}

		iRet = getMatrixOfDouble(piAddr2, &iRows, &iCols, &pdblPrec);
		if(iRet)
		{
			return 1;
		}
	
		if(!isVarComplex(piAddr1)) //Not complex
		{
			piBool[0] = 1;
		}
		else //Complex
		{
			switch(getVarType(piAddr1))
			{
			case sci_matrix :
				iRet = isreal_double(piAddr1, pdblPrec[0], piBool);
				break;
			case sci_poly :
				iRet = isreal_poly(piAddr1, pdblPrec[0], piBool);
				break;
			case sci_sparse :
				iRet = isreal_sparse(piAddr1, pdblPrec[0], piBool);
				break;
			default: //never pass here
				break;
			}
		}
	}

	if(iRet)
	{
		return 1;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int isreal_double(int* _piAddress, double _dblRef, int* _piIsReal)
{
	int iRet					= 0;
	int iRows					= 0;
	int iCols					= 0;

	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	iRet = getComplexMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
	if(iRet)
	{
		return 1;
	}

	iRet = isreal_common(pdblImg, iCols * iRows, _dblRef, _piIsReal);
	if(iRet)
	{
		return 1;
	}
	return 0;
}

int isreal_poly(int* _piAddress, double _dblRef, int* _piIsReal)
{
	int i;
	int iRet					= 0;
	int iRows					= 0;
	int iCols					= 0;
	int *piCoeff			= NULL;

	double** pdblReal	= NULL;
	double** pdblImg	= NULL;

	iRet = getComplexMatrixOfPoly(_piAddress, &iRows, &iCols, NULL, NULL, NULL);
	if(iRet)
	{
		return 1;
	}

	piCoeff = (int*)malloc(sizeof(int) * iRows * iCols);
	iRet = getComplexMatrixOfPoly(_piAddress, &iRows, &iCols, piCoeff, NULL, NULL);
	if(iRet)
	{
		return 1;
	}

	pdblReal	= (double**)malloc(sizeof(double*) * iRows * iCols);
	pdblImg		= (double**)malloc(sizeof(double*) * iRows * iCols);

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pdblReal[i]	= (double*)malloc(sizeof(double) * piCoeff[i]);
		pdblImg[i]	= (double*)malloc(sizeof(double) * piCoeff[i]);
	}

	iRet = getComplexMatrixOfPoly(_piAddress, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
	if(iRet)
	{
		return 1;
	}
	
	for(i = 0 ; i < iRows * iCols ; i++)
	{
		iRet = isreal_common(pdblImg[i], piCoeff[i], _dblRef, _piIsReal);
		if(iRet)
		{
			return 1;
		}

		if(*_piIsReal == 0)
		{
			break;
		}
	}
	return 0;
}

int isreal_sparse(int* _piAddress, double _dblRef, int* _piIsReal)
{
	int iRet					= 0;
	int iRows					= 0;
	int iCols					= 0;
	int iNbItem				= 0;
	int *piNbItemRow	= NULL;
	int *piColPos			= NULL;

	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	iRet = getComplexSparseMatrix(_piAddress, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
	if(iRet)
	{
		return 1;
	}

	iRet = isreal_common(pdblImg, iNbItem, _dblRef, _piIsReal);
	if(iRet)
	{
		return 1;
	}

	return 0;
}

int isreal_common(double *_pdblData, int _iSize, double _dblRef, int* _piIsReal)
{
	int i;
	*_piIsReal = 1;
	for(i = 0 ; i < _iSize ; i++)
	{
		if(_pdblData[i] > _dblRef)
		{
			*_piIsReal = 0;
			break;
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
