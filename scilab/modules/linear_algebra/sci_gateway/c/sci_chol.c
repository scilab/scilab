
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include <stdio.h>
#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"

int iRealCholProduct(double *_pdblReal, int _iLeadDim);
int iComplexCholProduct(doublecomplex *_poIn, int _iLeadDim);


/*--------------------------------------------------------------------------*/
extern int C2F(dpotrf)();
extern int C2F(zpotrf)();
extern int C2F(intdpotrf)(char *fname, unsigned long fname_len);
extern int C2F(intzpotrf)(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int C2F(intchol)(char *fname,unsigned long fname_len)
{
	int iCholProductResult = 0;
	int iRows	= 0;
	int iCols	= 0;

	int iReal	= 0;
	int iImg	= 0;

	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	double *pdblReturnReal	= NULL;
	double *pdblReturnImg	= NULL;

	if (GetType(1) != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	GetVarDimension(1, &iRows, &iCols);
	if(iRows != iCols)
	{
		Err = 1;
		Error(20);
		return 0;
	}

	if(iRows == 0)
	{
		LhsVar(1) = 1;
		return 0;
	}
	else if(iRows == -1) // manage eye case
	  // TODO : eye must not be a particular case with dimensions.
	{
		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iReal);
		pdblReal		= stk(iReal);
		if (pdblReal[0] <= 0)
		  {
		    /* Matrix must be positive definite */
		    Error(29);
		    return 0;
		  }
		pdblReal[0]		= sqrt(pdblReal[0]);
		LhsVar(1) = 1;
		return 0;
	}

	if(iIsComplex(1))
	{
		int iComplex	= 1;
		doublecomplex *poData = NULL;
		GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iReal, &iImg);
		pdblReal		= stk(iReal);
		pdblImg			= stk(iImg);

		poData = oGetDoubleComplexFromPointer(pdblReal, pdblImg, iRows * iCols);
		iCholProductResult = iComplexCholProduct(poData, iRows);

		iAllocComplexMatrixOfDouble(Rhs + 1, iRows, iCols, &pdblReturnReal, &pdblReturnImg);

		vGetPointerFromDoubleComplex(poData, iRows * iCols, pdblReturnReal, pdblReturnImg);
		vFreeDoubleComplexFromPointer(poData);
	}
	else
	{
		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iReal);
		pdblReal		= stk(iReal);

		iAllocMatrixOfDouble(Rhs + 1, iRows, iCols, &pdblReturnReal);

		memcpy(pdblReturnReal, pdblReal, iRows * iCols * sizeof(double));

		iCholProductResult = iRealCholProduct(pdblReturnReal, iRows);
	}

	if (iCholProductResult > 0)
	  {
	    /* Matrix must be positive definite */
	    Error(29);
	    return 0;
	  }

	LhsVar(1) = Rhs + 1;
	return 0;
}

int iRealCholProduct(double *_pdblReal, int _iLeadDim)
{
	char cOrient = 'U';
	int iInfo;

	C2F(dpotrf)(&cOrient, &_iLeadDim, _pdblReal, &_iLeadDim, &iInfo);
	if(iInfo > 0)
	  {
	    return iInfo;
	  }

	if(iInfo == 0 && _iLeadDim > 1)
	{
		int iIndex1 = 0, iIndex2 = 0;
		for(iIndex1 = 0 ; iIndex1 < _iLeadDim ; iIndex1++)
		{
			for(iIndex2 = iIndex1 + 1 ; iIndex2 < _iLeadDim ; iIndex2++)
			{
				_pdblReal[iIndex2 + iIndex1 * _iLeadDim] = 0;
			}
		}
	}

	return 0;
}

int iComplexCholProduct(doublecomplex *_poIn, int _iLeadDim)
{
	char cOrient = 'U';
	int iInfo;

	C2F(zpotrf)(&cOrient, &_iLeadDim, _poIn, &_iLeadDim, &iInfo);
	if(iInfo > 0)
	  {
	    return iInfo;
	  }

	if(iInfo == 0 && _iLeadDim > 1)
	{
		int iIndex1 = 0, iIndex2 = 0;
		for(iIndex1 = 0 ; iIndex1 < _iLeadDim ; iIndex1++)
		{
			for(iIndex2 = iIndex1 + 1 ; iIndex2 < _iLeadDim ; iIndex2++)
			{
				_poIn[iIndex2 + iIndex1 * _iLeadDim].r = 0;
				_poIn[iIndex2 + iIndex1 * _iLeadDim].i = 0;
			}
		}
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
