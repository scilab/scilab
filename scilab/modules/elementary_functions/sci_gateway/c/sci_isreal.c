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
#include "api_oldstack.h"

int isreal_double(int* _piKey, int* _piAddress, double _dblRef, int* _piIsReal);
int isreal_poly(int* _piKey, int* _piAddress, double _dblRef, int* _piIsReal);
int isreal_sparse(int* _piKey, int* _piAddress, double _dblRef, int* _piIsReal);
int isreal_common(double *_pdblData, int _iSize, double _dblRef, int* _piIsReal);

/*--------------------------------------------------------------------------*/
int sci_isreal(char *fname,int* _piKey)
{
	SciErr sciErr;
	int iRet			= 0;
	
	int iRows			= 0;
	int iCols			= 0;
	int* piAddr1	= NULL;

	int iType2 		= 0;
	int* piAddr2	= NULL;
	int iBool			= 0;

	CheckRhs(1,2);
	CheckLhs(1,1);

	sciErr  = getVarAddressFromPosition(_piKey, 1, &piAddr1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}


	if(!isDoubleType(_piKey, piAddr1) && !isPolyType(_piKey, piAddr1) && !isSparseType(_piKey, piAddr1))
	{
		OverLoad(1);
		return 0;
	}

	if(Rhs == 1)
	{
		if(isVarComplex(_piKey, piAddr1))
			iBool = 0;
		else
			iBool = 1;
	}
	else //Rhs == 2
	{
		if(!isVarComplex(_piKey, piAddr1)) //Not complex
		{
			iBool = 1;
		}
		else //Complex
		{
			int iType1 			= 0;
			double dblPrec 	= 0;
			
			sciErr = getVarAddressFromPosition(_piKey, 2, &piAddr2);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			iRet = getScalarDouble(_piKey, piAddr2, &dblPrec);
			if(iRet)
			{
				return 0;
			}

			sciErr = getVarType(_piKey, piAddr1, &iType1);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
		
			switch(iType1)
			{
			case sci_matrix :
				iRet = isreal_double(_piKey, piAddr1, dblPrec, &iBool);
				break;
			case sci_poly :
				iRet = isreal_poly(_piKey, piAddr1, dblPrec, &iBool);
				break;
			case sci_sparse :
				iRet = isreal_sparse(_piKey, piAddr1, dblPrec, &iBool);
				break;
			default: //never come here
				break;
			}
		}
	}

	if(iRet)
	{
		return 1;
	}

	iRet = createScalarBoolean(_piKey, Rhs + 1, iBool);
	if(iRet)
	{
		return 1;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int isreal_double(int* _piKey, int* _piAddress, double _dblRef, int* _piIsReal)
{
	SciErr sciErr;
	int iRet					= 0;
	
	int iRows					= 0;
	int iCols					= 0;

	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	sciErr = getComplexMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	iRet = isreal_common(pdblImg, iCols * iRows, _dblRef, _piIsReal);
	if(iRet)
	{
		return iRet;
	}
	
	return 0;
}

int isreal_poly(int* _piKey, int* _piAddress, double _dblRef, int* _piIsReal)
{
	SciErr sciErr;
	int i;
	
	int iRet					= 0;
	int iRows					= 0;
	int iCols					= 0;
	int *piCoeff			= NULL;

	double** pdblReal	= NULL;
	double** pdblImg	= NULL;

	iRet = getAllocatedMatrixOfComplexPoly(_piKey, _piAddress, &iRows, &iCols, &piCoeff, &pdblReal, &pdblImg);
	if(iRet)
	{
		freeAllocatedMatrixOfComplexPoly(iRows, iCols, piCoeff, pdblReal, pdblImg);
		return iRet;
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		iRet = isreal_common(pdblImg[i], piCoeff[i], _dblRef, _piIsReal);
		if(sciErr.iErr)
		{
			freeAllocatedMatrixOfComplexPoly(iRows, iCols, piCoeff, pdblReal, pdblImg);
			printError(&sciErr, 0);
			return sciErr.iErr;
		}

		if(*_piIsReal == 0)
		{
			break;
		}
	}

	freeAllocatedMatrixOfComplexPoly(iRows, iCols, piCoeff, pdblReal, pdblImg);
	return 0;
}

int isreal_sparse(int* _piKey, int* _piAddress, double _dblRef, int* _piIsReal)
{
	int iRet					= 0;
	int iRows					= 0;
	int iCols					= 0;
	int iNbItem				= 0;
	int *piNbItemRow	= NULL;
	int *piColPos			= NULL;

	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	iRet = getAllocatedComplexSparseMatrix(_piKey, _piAddress, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
	if(iRet)
	{
		return iRet;
	}

	iRet = isreal_common(pdblImg, iNbItem, _dblRef, _piIsReal);
	if(iRet)
	{
		return iRet;
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
