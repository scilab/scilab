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

SciErr getStartPosition(int _iPos, int* _piStartPos);

SciErr diag_double(int* _piAddress, int _iStartPos);
int diag_poly(int* _piAddress, int _iStartPos);

extern int sci_pdiag(char *fname,unsigned long fname_len);

int C2F(sci_diag) (char *fname,unsigned long fname_len)
{
	SciErr sciErr;
	int iRows						= 0;
	int iCols						= 0;
	int iType						= 0;

	int iStartPos			= 0;
	int iMatrixSize			= 0;

	int* piAddr					= NULL;

	CheckRhs(1,2);
	CheckLhs(1,1);

	if(Rhs == 2)
	{
		sciErr = getStartPosition(2, &iStartPos);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}

	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(pvApiCtx, piAddr, &iType);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	switch(iType)
	{
	case sci_matrix :
		sciErr = diag_double(piAddr, iStartPos);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		break;
	case sci_poly :
		if(diag_poly(piAddr, iStartPos))
		{
			return 1;
		}
		break;
	default :
		OverLoad(1);
		return 0;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

SciErr diag_double(int* _piAddress, int _iStartPos)
{
	SciErr sciErr;
	int iRows						= 0;
	int iCols						= 0;
	int iMatrixSize			= 0;

	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	double *pdblRealRet = NULL;
	double *pdblImgRet	= NULL;

	if(isVarComplex(pvApiCtx, _piAddress))
	{
		sciErr = getComplexMatrixOfDouble(pvApiCtx, _piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}
	else
	{
		sciErr = getMatrixOfDouble(pvApiCtx, _piAddress, &iRows, &iCols, &pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
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

			sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iRows, iCols, &pdblRealRet);
			return sciErr;
		}
		else
		{
			int iIncIn	= iRows + 1;
			int iIncOut = 1;
			if(isVarComplex(pvApiCtx, _piAddress))
			{
				sciErr = allocComplexMatrixOfDouble(pvApiCtx, Rhs + 1, iMatrixSize, 1, &pdblRealRet, &pdblImgRet);
			}
			else
			{
				sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iMatrixSize, 1, &pdblRealRet);
			}

			if(sciErr.iErr)
			{
				return sciErr;
			}

			if(_iStartPos >= 0)
			{
				double *pdblStartAddr = pdblReal + _iStartPos * iRows;
				C2F(unsfdcopy)(&iMatrixSize, pdblStartAddr, &iIncIn, pdblRealRet, &iIncOut);

				if(isVarComplex(pvApiCtx, _piAddress))
				{
					pdblStartAddr = pdblImg + _iStartPos * iRows;
					C2F(unsfdcopy)(&iMatrixSize, pdblStartAddr, &iIncIn, pdblImgRet, &iIncOut);
				}
			}
			else
			{
				double *pdblStartAddr = pdblReal - _iStartPos;
				C2F(unsfdcopy)(&iMatrixSize, pdblStartAddr, &iIncIn, pdblRealRet, &iIncOut);

				if(isVarComplex(pvApiCtx, _piAddress))
				{
					pdblStartAddr = pdblImg - _iStartPos;
					C2F(unsfdcopy)(&iMatrixSize, pdblStartAddr, &iIncIn, pdblImgRet, &iIncOut);
				}
				iRows = iMatrixSize;
				iCols = 1;
			}
			return sciErr;
		}
	}
	else
	{//iRows == 1 || iCols == 1
		int iIncIn			= 1;
		int iIncOut			= 0;
		int iOriginalSize	= iRows * iCols;

		iMatrixSize = Max(iRows, iCols) + (int)dabss(_iStartPos);
		iIncOut = iMatrixSize + 1;

		if(isVarComplex(pvApiCtx, _piAddress))
		{
			sciErr = allocComplexMatrixOfDouble(pvApiCtx, Rhs + 1, iMatrixSize, iMatrixSize, &pdblRealRet, &pdblImgRet);
			if(sciErr.iErr)
			{
				return sciErr;
			}

			memset(pdblImgRet, 0x00, iMatrixSize * iMatrixSize * sizeof(double));
		}
		else
		{
			sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iMatrixSize, iMatrixSize, &pdblRealRet);
			if(sciErr.iErr)
			{
				return sciErr;
			}
		}

		memset(pdblRealRet, 0x00, iMatrixSize * iMatrixSize * sizeof(double));

		if(_iStartPos >= 0)
		{
			double *pdblStartAddr = pdblRealRet + iMatrixSize * _iStartPos;
			C2F(unsfdcopy)(&iOriginalSize, pdblReal, &iIncIn, pdblStartAddr, &iIncOut);

			if(isVarComplex(pvApiCtx, _piAddress))
			{
				pdblStartAddr = pdblImgRet + iMatrixSize * _iStartPos;
				C2F(unsfdcopy)(&iOriginalSize, pdblImg, &iIncIn, pdblStartAddr, &iIncOut);
			}
		}
		else
		{
			double *pdblStartAddr = pdblRealRet - _iStartPos;
			C2F(unsfdcopy)(&iOriginalSize, pdblReal, &iIncIn, pdblStartAddr, &iIncOut);

			if(isVarComplex(pvApiCtx, _piAddress))
			{
				pdblStartAddr = pdblImgRet - _iStartPos;
				C2F(unsfdcopy)(&iOriginalSize, pdblImg, &iIncIn, pdblStartAddr, &iIncOut);
			}
		}
	}
	return sciErr;
}

int diag_poly(int* _piAddress, int _iStartPos)
{
	sci_pdiag("diag", 4);
	return 0;
}

SciErr getStartPosition(int _iPos, int* _piStartPos)
{
	SciErr sciErr;
	int iRows					= 0;
	int iCols					= 0;
	int* piAddr				= NULL;
	double* pdblReal	= NULL;

	sciErr = getVarAddressFromPosition(pvApiCtx, _iPos, &piAddr);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	if(iRows != 1 || iCols != 1)
	{
		Error(89);
	}

	*_piStartPos = (int)pdblReal[0];
	return sciErr;
}
/*--------------------------------------------------------------------------*/
