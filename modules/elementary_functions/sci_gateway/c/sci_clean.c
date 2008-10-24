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


/*Some call to other module ( polynomial et sparse*/
extern int C2F(sci_cleanp) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_spclean) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(ref2val) _PARAMS((void));
extern double C2F(dasum)();

int C2F(sci_clean) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
	int iRows1		= 0;
	int iCols1		= 0;
	int iSize1		= 0;
	int iRealData1	= 0;
	int iImgData1	= 0;

	int iRows2		= 0;
	int iCols2		= 0;
	int iSize2		= 0;
	int iRealData2	= 0;
	int iImgData2	= 0;

	int iRows3		= 0;
	int iCols3		= 0;
	int iSize3		= 0;
	int iRealData3	= 0;
	int iImgData3	= 0;

	int iIndex		= 0;
	int iComplex	= 1;

	double dblEpsR	= 1E-10;
	double dblEpsA	= 1E-10;
	double dblEps	= 0;

	double	*pReturnRealData= NULL;
	double	*pReturnImgData	= NULL;
	double	*pdblRealData	= 0;
	double	*pdblImgData	= 0;

	double dblNorm = 0;

	int iCurrentVar = 3;
	CheckRhs(1,3);
	CheckLhs(1,1);

	if(GetType(1) == sci_poly)
	{
		C2F(sci_cleanp)(fname, fname_len);
		return 0;
	}
	else if(GetType(1) == sci_sparse)
	{
		C2F(ref2val)();
		C2F(sci_spclean)(fname, fname_len);
		return 0;
	}
	else if(GetType(1) != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	if(Rhs == 3)
	{
		if(GetType(iCurrentVar) != sci_matrix)
		{
			Error(52);
			return 0;
		}

		if(iIsComplex(iCurrentVar) == 1)
		{
			Error(52);
			return 0;
		}

		GetRhsVar(iCurrentVar, MATRIX_OF_DOUBLE_DATATYPE, &iRows3, &iCols3, &iRealData3);
		if(iRows3 != 1 || iCols3 != 1)
		{
			Error(60);
			return 0;
		}
		iSize3 = iRows3 * iCols3;
		dblEpsR = stk(iRealData3)[0];
	}

	iCurrentVar--; //2 donc
	if(Rhs >= 2)
	{
		if(GetType(iCurrentVar) != sci_matrix)
		{
			Error(52);
			return 0;
		}

		if(iIsComplex(iCurrentVar) == 1)
		{
			Error(52);
			return 0;
		}

		GetRhsVar(iCurrentVar, MATRIX_OF_DOUBLE_DATATYPE, &iRows2, &iCols2, &iRealData2);
		if(iRows2 != 1 || iCols2 != 1)
		{
			Error(60);
			return 0;
		}
		iSize2 = iRows2 * iCols2;
		dblEpsA = stk(iRealData2)[0];
	}
	iCurrentVar--; //1 donc

	if(iIsComplex(iCurrentVar))
	{
		GetRhsVar(iCurrentVar, MATRIX_OF_DOUBLE_DATATYPE, &iRows1, &iCols1, &iRealData1);
		pdblRealData	= stk(iRealData1);
		pdblImgData		= stk(iImgData1);
		dblNorm			=
		dblNorm			= wasums(iRows1 * iCols1, pdblRealData, pdblImgData);

		iAllocComplexMatrixOfDouble(Rhs + 1, 1, iRows1, iCols1, &pReturnRealData, &pReturnImgData);
		//pReturnRealData = (double*)malloc(iRows1 * iCols1 * sizeof(double));
		//pReturnImgData	= (double*)malloc(iRows1 * iCols1 * sizeof(double));

		dblEps = Max(dblEpsA, dblEpsR * dblNorm);
		for(iIndex = 0 ; iIndex < iRows1 * iCols1 ; iIndex++)
		{
			pReturnRealData[iIndex] = dabss(pdblRealData[iIndex]) <= dblEps ? 0 : pdblRealData[iIndex];
			pReturnImgData[iIndex]	= dabss(pdblImgData[iIndex]) <= dblEps ? 0 : pdblImgData[iIndex];
		}

		//CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows1, &iCols1, &pReturnRealData, &pReturnImgData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		//free(pReturnRealData);
		//free(pReturnImgData);
	}
	else
	{
		int iOne	= 1;

		GetRhsCVar(iCurrentVar, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows1, &iCols1, &iRealData1, &iImgData1);
		pdblRealData	= stk(iRealData1);

		iSize1			= iRows1 * iCols1;
		dblNorm			= C2F(dasum)(&iSize1, pdblRealData, &iOne);

		dblEps = Max(dblEpsA, dblEpsR * dblNorm);
		iAllocMatrixOfDouble(Rhs + 1, iRows1, iCols1, &pReturnRealData);
		//pReturnRealData = (double*)malloc(iRows1 * iCols1 * sizeof(double));
		for(iIndex = 0 ; iIndex < iRows1 * iCols1 ; iIndex++)
			pReturnRealData[iIndex] = dabss(pdblRealData[iIndex]) <= dblEps ? 0 : pdblRealData[iIndex];

		//CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows1, &iCols1, &pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		//free(pReturnRealData);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
