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

int getStartPosition(int _iPos, int* _piStartPos);

int diag_double(int* _piAddress, int _iStartPos);
int diag_poly(int* _piAddress, int _iStartPos);

extern int C2F(sci_pdiag) (char *fname,unsigned long fname_len);

int C2F(sci_diag) (char *fname,unsigned long fname_len)
{
	int iRet						= 0;
	int iRows						= 0;
	int iCols						= 0;

	int _iStartPos				= 0;
	int iMatrixSize			= 0;

	int* piAddr					= NULL;

	CheckRhs(1,2);
	CheckLhs(1,1);

	if(Rhs == 2)
	{
		iRet = getStartPosition(2, &_iStartPos);
		if(iRet)
		{
			return 1;
		}
	}

	iRet = getVarAddressFromPosition(1, &piAddr);
	if(iRet)
	{
		return 1;
	}

	switch(getVarType(piAddr))
	{
	case sci_matrix :
		iRet = diag_double(piAddr, _iStartPos);
		break;
	case sci_poly :
		iRet = diag_poly(piAddr, _iStartPos);
		break;
	default :
		OverLoad(1);
		return 0;
	}

	if(iRet)
	{
		return 1;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int diag_double(int* _piAddress, int _iStartPos)
{
	int iRet						= 0;
	int iRows						= 0;
	int iCols						= 0;
	int iMatrixSize			= 0;

	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	double *pdblRealRet = NULL;
	double *pdblImgRet	= NULL;

	if(isVarComplex(_piAddress))
	{
		iRet = getComplexMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
		if(iRet)
		{
			return 1;
		}
	}
	else
	{
		iRet = getMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblReal);
		if(iRet)
		{
			return 1;
		}
	}

	if(iRows != 1 && iCols != 1)
	{
		if(_iStartPos >= 0)
			iMatrixSize = Max(0, Min(iRows, iCols - _iStartPos));
		else
			iMatrixSize = Max(0, Min(iRows + _iStartPos, iCols));

		if(iMatrixSize == 0)
		{
			iRows = 0;
			iCols = 0;

			iRet = allocMatrixOfDouble(Rhs + 1, iRows, iCols, &pdblRealRet);
			if(iRet)
			{
				return 1;
			}
			return 0;
		}
		else
		{
			int iIncIn	= iRows + 1;
			int iIncOut = 1;
			if(isVarComplex(_piAddress))
			{
				iRet = allocComplexMatrixOfDouble(Rhs + 1, iMatrixSize, 1, &pdblRealRet, &pdblImgRet);
			}
			else
			{
				iRet = allocMatrixOfDouble(Rhs + 1, iMatrixSize, 1, &pdblRealRet);
			}

			if(iRet)
			{
				return 1;
			}

			if(_iStartPos >= 0)
			{
				double *pdblStartAddr = pdblReal + _iStartPos * iRows;
				C2F(unsfdcopy)(&iMatrixSize, pdblStartAddr, &iIncIn, pdblRealRet, &iIncOut);

				if(isVarComplex(_piAddress))
				{
					pdblStartAddr = pdblImg + _iStartPos * iRows;
					C2F(unsfdcopy)(&iMatrixSize, pdblStartAddr, &iIncIn, pdblImgRet, &iIncOut);
				}
			}
			else
			{
				double *pdblStartAddr = pdblReal - _iStartPos;
				C2F(unsfdcopy)(&iMatrixSize, pdblStartAddr, &iIncIn, pdblRealRet, &iIncOut);

				if(isVarComplex(_piAddress))
				{
					pdblStartAddr = pdblImg - _iStartPos;
					C2F(unsfdcopy)(&iMatrixSize, pdblStartAddr, &iIncIn, pdblImgRet, &iIncOut);
				}
				iRows = iMatrixSize;
				iCols = 1;
			}
			return 0;
		}
	}
	else
	{//iRows == 1 || iCols == 1
		int iIncIn			= 1;
		int iIncOut			= 0;
		int iOriginalSize	= iRows * iCols;

		iMatrixSize = Max(iRows, iCols) + (int)dabss(_iStartPos);
		iIncOut = iMatrixSize + 1;

		if(isVarComplex(_piAddress))
		{
			iRet = allocComplexMatrixOfDouble(Rhs + 1, iMatrixSize, iMatrixSize, &pdblRealRet, &pdblImgRet);
			if(iRet)
			{
				return 1;
			}

			memset(pdblImgRet, 0x00, iMatrixSize * iMatrixSize * sizeof(double));
		}
		else
		{
			iRet = allocMatrixOfDouble(Rhs + 1, iMatrixSize, iMatrixSize, &pdblRealRet);
			if(iRet)
			{
				return 1;
			}
		}

		memset(pdblRealRet, 0x00, iMatrixSize * iMatrixSize * sizeof(double));

		if(_iStartPos >= 0)
		{
			double *pdblStartAddr = pdblRealRet + iMatrixSize * _iStartPos;
			C2F(unsfdcopy)(&iOriginalSize, pdblReal, &iIncIn, pdblStartAddr, &iIncOut);

			if(isVarComplex(_piAddress))
			{
				pdblStartAddr = pdblImgRet + iMatrixSize * _iStartPos;
				C2F(unsfdcopy)(&iOriginalSize, pdblImg, &iIncIn, pdblStartAddr, &iIncOut);
			}
		}
		else
		{
			double *pdblStartAddr = pdblRealRet - _iStartPos;
			C2F(unsfdcopy)(&iOriginalSize, pdblReal, &iIncIn, pdblStartAddr, &iIncOut);

			if(isVarComplex(_piAddress))
			{
				pdblStartAddr = pdblImgRet - _iStartPos;
				C2F(unsfdcopy)(&iOriginalSize, pdblImg, &iIncIn, pdblStartAddr, &iIncOut);
			}
		}
	}
	return 0;
}

int diag_poly(int* _piAddress, int _iStartPos)
{
	C2F(sci_pdiag)("diag", 4);
	return 0;
}

int getStartPosition(int _iPos, int* _piStartPos)
{
	int iRet					= 0;
	int iRows					= 0;
	int iCols					= 0;
	int* piAddr				= NULL;
	double* pdblReal	= NULL;

	iRet = getVarAddressFromPosition(_iPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	if(isVarComplex(piAddr))
	{
		Error(52);
		return 0;
	}

	iRet = getMatrixOfDouble(piAddr, &iRows, &iCols, &pdblReal);
	if(iRet)
	{
		return 1;
	}

	if(iRows != 1 || iCols != 1)
	{
		Error(89);
		return 0;
	}
	*_piStartPos = (int)pdblReal[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
