/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Antoine ELIAS <antoine.elias@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "double.h"
#include "Scierror.h"
		
int matmult()
{
	int iRows1 = 0, iRows2 = 0, iCols1 = 0, iCols2 = 0;
	int iReal1 = 0, iReal2 = 0, iImg1 = 0, iImg2 = 0;
	int iSize1 = 0, iSize2 = 0;
	int iGlobalComplex = 0;
	int iOne	= 1;


	double *pReal1 = NULL, *pReal2 = NULL, *pImg1 = NULL, *pImg2 = NULL;
	int iComplex1 = 0, iComplex2 = 0;

	double *pReturnReal = NULL, *pReturnImg = NULL;


	if(iIsComplex(1))
	{
		iComplex1 = 1;
		GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex1, &iRows1, &iCols1, &iReal1, &iImg1);
		pReal1	= stk(iReal1);
		pImg1	= stk(iImg1);
	}
	else
	{
		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows1, &iCols1, &iReal1);
		pReal1	= stk(iReal1);
	}

	if(iIsComplex(2))
	{
		iComplex2 = 1;
		GetRhsCVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iComplex2, &iRows2, &iCols2, &iReal2, &iImg2);
		pReal2	= stk(iReal2);
		pImg2	= stk(iImg2);
	}
	else
	{
		GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows2, &iCols2, &iReal2);
		pReal2	= stk(iReal2);
	}

	iSize1 = iRows1 * iCols1;
	iSize2 = iRows2 * iCols2;

	//if one is complex, all is complex !
	iGlobalComplex = Max(iComplex1, iComplex2);

	//Here we have retrieve all data
	if(iSize1 == 0 || iSize2 == 0)
	{
		double *pReturnData;
		iAllocMatrixOfDouble(Rhs + 1, 0, 0, &pReturnData);
	}

	if(iSize1 == 1)
	{// cst*a
		if(iRows1 < 0)
		{//eye*cst
			if(iSize2 == 1)
			{
				if(iGlobalComplex)
				{
					iAllocComplexMatrixOfDouble(Rhs + 1, iRows1, iCols1, &pReturnReal, &pReturnImg);
				}
				else
				{
					iAllocMatrixOfDouble(Rhs + 1, iRows1, iCols1, &pReturnReal);
				}
			}
			else
			{
				Error(14);
				return 0;
			}
		}
		else
		{
			if(iGlobalComplex)
			{
				iAllocComplexMatrixOfDouble(Rhs + 1, iRows2, iCols2, &pReturnReal, &pReturnImg);
			}
			else
			{
				iAllocMatrixOfDouble(Rhs + 1, iRows2, iCols2, &pReturnReal);
			}
		}

		if(iComplex1 == 0 && iComplex2 == 0)
		{//Matrix and scalar are real
			iMultiRealScalarByRealMatrix(
				pReal1[0], 
				pReal2, iRows2, iCols2,
				pReturnReal);
		}
		else if(iComplex1 == 0 && iComplex2 == 1)
		{//Matrix is complex and scalar is real
			iMultiRealScalarByComplexMatrix(
				pReal1[0],
				pReal2, pImg2, iRows2, iCols2,
				pReturnReal, pReturnImg);
		}
		else if(iComplex1 == 1 && iComplex2 == 0)
		{//Matrix is real and scalar are complex
			iMultiComplexScalarByRealMatrix(
				pReal1[0], pImg1[0],
				pReal2, iRows2, iCols2,
				pReturnReal, pReturnImg);
		}
		else if(iComplex1 == 1 && iComplex2 == 1)
		{//Matrix and scalar are complex
			iMultiComplexScalarByComplexMatrix(
				pReal1[0], pImg1[0],
				pReal2, pImg2, iRows2, iCols2,
				pReturnReal, pReturnImg);
		}
	}
	else if (iSize2 == 1)
	{//a*cst
		if(iRows2 < 0)
		{
			Error(14);
			return 0;
		}

		if(iGlobalComplex)
		{
			iAllocComplexMatrixOfDouble(Rhs + 1, iRows1, iCols1, &pReturnReal, &pReturnImg);
		}
		else
		{
			iAllocMatrixOfDouble(Rhs + 1, iRows1, iCols1, &pReturnReal);
		}
		if(iComplex1 == 0 && iComplex2 == 0)
		{//Matrix and scalar are real
			iMultiRealScalarByRealMatrix(
				pReal2[0], 
				pReal1, iRows1, iCols1,
				pReturnReal);
		}
		else if(iComplex1 == 0 && iComplex2 == 1)
		{//Matrix is real and scalar is complex
			iMultiComplexScalarByRealMatrix(
				pReal2[0], pImg2[0],
				pReal1, iRows1, iCols1,
				pReturnReal, pReturnImg);
		}
		else if(iComplex1 == 1 && iComplex2 == 0)
		{//Matrix is complex and scalar is real
			iMultiRealScalarByComplexMatrix(
				pReal2[0],
				pReal1, pImg1, iRows1, iCols1,
				pReturnReal, pReturnImg);
		}
		else if(iComplex1 == 1 && iComplex2 == 1)
		{//Matrix and scalar are complex
			iMultiComplexScalarByComplexMatrix(
				pReal2[0], pImg2[0],
				pReal1, pImg1, iRows1, iCols1,
				pReturnReal, pReturnImg);
		}
	}
	else
	{//matrix * matrix
		if(iCols1 != iRows2)
		{
			Error(10);
			return 0;
		}

		if(iGlobalComplex)
		{
			iAllocComplexMatrixOfDouble(Rhs + 1, iRows1, iCols2, &pReturnReal, &pReturnImg);
		}
		else
		{
			iAllocMatrixOfDouble(Rhs + 1, iRows1, iCols2, &pReturnReal);
		}
	
		if(iComplex1 == 1 && iComplex2 == 1)
		{//Both matrix are complex

			iMultiComplexMatrixByComplexMatrix(
					pReal1, pImg1, iRows1, iCols1,
					pReal2, pImg2, iRows2, iCols2,
					pReturnReal, pReturnImg);
		}
		else
		{
			double dblOne		= 1;
			double dblMinusOne	= -1;
			double dblZero		= 0;

			C2F(dgemm)("n", "n", &iRows1, &iCols2, &iCols1, &dblOne, pReal1 , &iRows1 , pReal2, &iRows2, &dblZero, pReturnReal ,&iRows1);
			if(iComplex1 == 1)
				C2F(dgemm)("n", "n", &iRows1, &iCols2, &iCols1, &dblOne, pImg1 , &iRows1 , pReal2, &iRows2, &dblZero, pReturnImg ,&iRows1);
			if(iComplex2 == 1)
				C2F(dgemm)("n", "n", &iRows1, &iCols2, &iCols1, &dblOne, pReal1 , &iRows1 , pImg2, &iRows2, &dblZero, pReturnImg ,&iRows1);

		}
	}
	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
