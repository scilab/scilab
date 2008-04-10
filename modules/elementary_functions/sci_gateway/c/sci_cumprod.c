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

int C2F(sci_cumprod) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
	int iRows			= 0;
	int iCols			= 0;
	int iRealData		= 0;
	char **szRealData	= 0;
	int iImgData		= 0;
	int iIndex			= 0;
	int iMode			= 0;

	CheckRhs(1,2);
	CheckLhs(1,1);

	if(GetType(1) != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	if(Rhs == 2)
	{
		double dblSel = 0;
		double *pdblRealData = 0;

		if(GetType(2) == sci_matrix)
		{
			GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
			iMode = (int)stk(iRealData)[0];
		}
		else if(GetType(2) == sci_strings)
		{
			GetRhsVar(2, MATRIX_OF_STRING_DATATYPE, &iRows, &iCols, &szRealData);
			iMode = (int)*szRealData[0];
		}
		else
		{
			Error(44);
			return 2;
		}
		
		if(iRows != 1 || iCols != 1)
		{
			Error(89);
			return 2;
		}

		if(iMode == ROW_LETTER || iMode == BY_ROWS)
			iMode = BY_ROWS;
		else if(iMode == COL_LETTER || iMode == BY_COLS)
			iMode = BY_COLS;
		else if(iMode == STAR_LETTER || iMode == BY_ALL)
			iMode = BY_ALL;
		else if(iMode == MTLB_LETTER || iMode == BY_MTLB)
		{//J'ai pas tout compris dans le fonctionnement pour MtLb©
			iMode = 0;
			if(iRows > 1)
				iMode = 1;
			else if(iCols > 1)
				iMode = 2;
		}
		else
		{
			Error(44);
			return 2;
		}
	}


	if(iIsComplex(1))
	{
		double *pdblRealData = 0;
		double *pdblImgData = 0;
		double *pReturnRealData = NULL;
		double *pReturnImgData = NULL;
		int iComplex = 1;

		GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iRealData, &iImgData);
		pdblRealData	= stk(iRealData);
		pdblImgData		= stk(iImgData);

		pReturnRealData = (double*)malloc(iRows * iCols * sizeof(double));
		pReturnImgData	= (double*)malloc(iRows * iCols * sizeof(double));

		/*Set the first column of returned matrix at the same value of the input matrix*/
		for(iIndex = 0 ; iIndex < iRows ; iIndex++)
		{
			pReturnRealData[iIndex] = pdblRealData[iIndex];
			pReturnImgData[iIndex] = pdblImgData[iIndex];
		}

		if(iMode == BY_ROWS)
		{
			for(iIndex = 0 ; iIndex < iCols ; iIndex++)
				vCuproi(iRows, pdblRealData + iRows * iIndex, pdblImgData + iRows * iIndex, pReturnRealData + iRows * iIndex, pReturnImgData + iRows * iIndex);
		}
		else if(iMode == BY_COLS)
		{
			int iIndex = 0;
			int iOffset = 0;

			for(iIndex = 0 ; iIndex < iCols -1; iIndex++)
			{
				vWvmul(iRows, pReturnRealData + iOffset, pReturnImgData + iOffset, pdblRealData + iOffset + iRows, pdblImgData + iOffset + iRows, 1, 1, pReturnRealData + iOffset + iRows, pReturnImgData + iOffset + iRows);
				iOffset += iRows;
			}
		}
		else if(iMode == BY_ALL)
		{
			vCuproi(iRows * iCols, pdblRealData, pdblImgData, pReturnRealData, pReturnImgData);
		}
		else
		{
			Error(44);
			return 2;
		}

		CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &pReturnRealData, &pReturnImgData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(pReturnRealData);
		free(pReturnImgData);
	}
	else
	{
		double *pdblRealData = 0;
		double *pReturnRealData = NULL;

		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		pdblRealData		= stk(iRealData);
		pReturnRealData		= (double*)malloc(iRows * iCols * sizeof(double));
		for(iIndex = 0 ; iIndex < iRows ; iIndex++)
			pReturnRealData[iIndex] = pdblRealData[iIndex];

		if(iMode == BY_ROWS)
		{
			for(iIndex = 0 ; iIndex < iCols ; iIndex++)
				vCupro(iRows, pdblRealData + iRows * iIndex, pReturnRealData + iRows * iIndex);
		}
		else if(iMode == BY_COLS)
		{
			int iIndex = 0;
			int iOffset = 0;

			for(iIndex = 0 ; iIndex < iCols -1; iIndex++)
			{
				vDvmul(iRows, pReturnRealData + iOffset, pdblRealData + iOffset + iRows, 1, 1, pReturnRealData + iOffset + iRows);
				iOffset += iRows;
			}
		}
		else if(iMode == BY_ALL)
		{
			vCupro(iRows * iCols, pdblRealData, pReturnRealData);
		}
		else
		{
			Error(44);
			return 2;
		}

		CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(pReturnRealData);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
