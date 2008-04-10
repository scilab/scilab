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

extern int C2F(sci_pdiag) _PARAMS((char *fname,unsigned long fname_len));

#define _NEW_TONIO_

int C2F(sci_diag) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_

	int iRows			= 0;
	int iCols			= 0;
	int iRealData		= 0;
	int iImgData		= 0;
	int iIndex			= 0;
	int iStartPos		= 0;
	int iMatrixSize		= 0;

	CheckRhs(1,2);
	CheckLhs(1,1);

	if(Rhs == 2)
	{
		if(iIsComplex(2) == 1)
		{
			Error(52);
			return 0;
		}
		GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);

		if(iRows * iCols != 1)
		{
			Error(89);
			return 0;
		}
		iStartPos = (int)stk(iRealData)[0];
	}


	if(GetType(1) != sci_matrix && GetType(1) != sci_poly)
	{
		OverLoad(1);
		return 0;
	}

	if(GetType(1) == sci_matrix)
	{
		double *pdblRealData = 0;
		double *pdblImgData = 0;
		double *pReturnRealData = NULL;
		double *pReturnImgData = NULL;

		int iComplex = iIsComplex(1);

		if(iComplex)
		{
			GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iRealData, &iImgData);
			pdblRealData = stk(iRealData);
			pdblImgData = stk(iImgData);
		}
		else
		{
			GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
			pdblRealData = stk(iRealData);
		}

		if(iRows != 1 && iCols != 1)
		{
			if(iStartPos >= 0)
				iMatrixSize = Max(0, Min(iRows, iCols - iStartPos));
			else
				iMatrixSize = Max(0, Min(iRows + iStartPos, iCols));

			pReturnRealData = (double*)malloc(iMatrixSize * sizeof(double));
			if(iComplex)
				pReturnImgData = (double*)malloc(iMatrixSize * sizeof(double));

			if(iMatrixSize == 0)
			{
				iRows = 0;
				iCols = 0;

				CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
				LhsVar(1) = Rhs + 1;
				PutLhsVar();
				free(pReturnRealData);
				return 0;
			}
			else
			{
				int iIncIn	= iRows + 1;
				int iIncOut = 1;

				if(iStartPos >= 0)
				{
					double *pdblStatAddr = pdblRealData + iStartPos * iRows;
					C2F(unsfdcopy)(&iMatrixSize, pdblStatAddr, &iIncIn, pReturnRealData, &iIncOut);
	
					if(iComplex)
					{
						pdblStatAddr = pdblImgData + iStartPos * iRows;
						C2F(unsfdcopy)(&iMatrixSize, pdblStatAddr, &iIncIn, pReturnImgData, &iIncOut);
					}
					iRows = iMatrixSize;
					iCols = 1;
				}
				else
				{
					double *pdblStatAddr = pdblRealData - iStartPos;
					C2F(unsfdcopy)(&iMatrixSize, pdblStatAddr, &iIncIn, pReturnRealData, &iIncOut);

					if(iComplex)
					{
						pdblStatAddr = pdblImgData - iStartPos;
						C2F(unsfdcopy)(&iMatrixSize, pdblStatAddr, &iIncIn, pReturnImgData, &iIncOut);
					}
					iRows = iMatrixSize;
					iCols = 1;
				}

				if(iComplex)
				{
					CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &pReturnRealData, &pReturnImgData);
					free(pReturnImgData);
				}
				else
				{
					CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
				}
				LhsVar(1) = Rhs + 1;
				PutLhsVar();
				free(pReturnRealData);
				return 0;
			}
		}
		else
		{//iRows == 1 || iCols == 1
			int iIncIn			= 1;
			int iIncOut			= 0;
			int iOriginalSize	= iRows * iCols;
			iMatrixSize = Max(iRows, iCols) + (int)dabss(iStartPos);

			iIncOut = iMatrixSize + 1;

			pReturnRealData = (double*)malloc(iMatrixSize * iMatrixSize * sizeof(double));
			memset(pReturnRealData, 0x00, iMatrixSize * iMatrixSize * sizeof(double));
			if(iComplex)
			{
				pReturnImgData = (double*)malloc(iMatrixSize * iMatrixSize * sizeof(double));
				memset(pReturnImgData, 0x00, iMatrixSize * iMatrixSize * sizeof(double));
			}


			if(iStartPos >= 0)
			{
				double *pdblStatAddr = pReturnRealData + iMatrixSize * iStartPos;
				C2F(unsfdcopy)(&iOriginalSize, pdblRealData, &iIncIn, pdblStatAddr, &iIncOut);

				if(iComplex)
				{
					pdblStatAddr = pReturnImgData + iMatrixSize * iStartPos;
					C2F(unsfdcopy)(&iOriginalSize, pdblImgData, &iIncIn, pdblStatAddr, &iIncOut);
				}
			}
			else
			{
				double *pdblStatAddr = pReturnRealData - iStartPos;
				C2F(unsfdcopy)(&iOriginalSize, pdblRealData, &iIncIn, pdblStatAddr, &iIncOut);

				if(iComplex)
				{
					pdblStatAddr = pReturnImgData - iStartPos;
					C2F(unsfdcopy)(&iOriginalSize, pdblImgData, &iIncIn, pdblStatAddr, &iIncOut);
				}
			}

			iRows = iMatrixSize;
			iCols = iMatrixSize;

			if(iComplex)
			{
				CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &pReturnRealData, &pReturnImgData);
				free(pReturnImgData);
			}
			else
			{
				CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
			}
			LhsVar(1) = Rhs + 1;
			PutLhsVar();
			free(pReturnRealData);
		}
	}
	else
	{//sci_poly
		C2F(sci_pdiag)(fname, fname_len);
	}
#else
	C2F(intdiag)(id);
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
