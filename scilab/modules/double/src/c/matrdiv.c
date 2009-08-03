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

#include <stdio.h>
#include "double.h"
#include "stack-c.h"

extern int C2F(error)(int *n);

int matrdiv()
{
	int iRows1	= 0, iRows2	= 0, iCols1	= 0, iCols2	= 0;
	int iReal1	= 0, iReal2 = 0, iImg1	= 0, iImg2	= 0;
	int iSize1	= 0, iSize2 = 0;

	int iGlobalComplex	= 0;
	int iRet			= 0;
	int iIndex			= 0;

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
		iAllocMatrixOfDouble(Rhs + 1, 0, 0, &pReturnReal);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		return 0;
	}
	else
	{
		if(iRows1 < 0)
		{
			Error(14);
			return 0;
		}

		if(iRows2 == 0)
		{
			LhsVar(1) = 1;
			PutLhsVar();
			return 0;
		}

		if(iCols1 != iCols2 && iSize2 != 1 && iSize1 != 1)
		{
			Error(266);
			return 0;
		}

		if(iRows1 == 0 || iCols1 == 0)
		{
			iAllocMatrixOfDouble(Rhs + 1, 0, 0, &pReturnReal);
			LhsVar(1) = Rhs + 1;
			PutLhsVar();
			return 0;
		}

		//Check if A and B matrices contains Inf or NaN's
		if(iComplex1)
		{
			for(iIndex = 0 ; iIndex < iRows1 * iCols1 ; iIndex++)
			{
				if(finite(pReal1[iIndex]) == 0 || finite(pImg1[iIndex]) == 0)
				{
					Error(229);
					return 0;
				}
			}
		}
		else
		{
			for(iIndex = 0 ; iIndex < iRows1 * iCols1 ; iIndex++)
			{
				if(finite(pReal1[iIndex]) == 0)
				{
					Error(229);
					return 0;
				}
			}
		}

		if(iComplex2)
		{
			for(iIndex = 0 ; iIndex < iRows2 * iCols2 ; iIndex++)
			{
				if(finite(pReal2[iIndex]) == 0 || finite(pImg2[iIndex]) == 0)
				{
					Error(229);
					return 0;
				}
			}
		}
		else
		{
			for(iIndex = 0 ; iIndex < iRows2 * iCols2 ; iIndex++)
			{
				if(finite(pReal2[iIndex]) == 0)
				{
					Error(229);
					return 0;
				}
			}
		}

		if(iSize2 != 1 /*&& iSize1 != 1*/)
		{
			if(iCols2 != iCols1 && iRows1 * iCols1 == 1)
			{
				C2F(com).fun=0;
				Fin=-Fin;
				return 0;
			}


			if(iComplex1 == 0 && iComplex2 == 0)
			{// A and B are both real
				double dblRcond = 0;
				iAllocMatrixOfDouble(Rhs + 1,  iRows1, iRows2, &pReturnReal);
				iRet = iRightDivisionOfRealMatrix(pReal1, iRows1, iCols1, pReal2, iRows2, iCols2, pReturnReal, iRows2, iRows1, &dblRcond);

				if(iRet > 0)
				{
					Error(iRet);
					return 0;
				}
				else if(iRet < 0)
				{
					switch(iRet)
					{
					case -1 :
						sprintf(C2F(cha1).buf, "%1.4E", dblRcond);
						Msgs(5,1);
						break;
					case -2 :
						Msgs(9, (int)dblRcond);
						break;
					default :
							break;
					}
				}

				LhsVar(1) = Rhs + 1;
				PutLhsVar();
			}
			else
			{/* A real, B complex : complexify A */
				double dblRcond = 0;
				iAllocComplexMatrixOfDouble(Rhs + 1, iRows1, iRows2, &pReturnReal, &pReturnImg);
				iRet = iRightDivisionOfComplexMatrix(
					pReal1, pImg1, iRows1, iCols1, 
					pReal2, pImg2, iRows2, iCols2, 
					pReturnReal, pReturnImg, iRows1, iRows2, &dblRcond);

				if(iRet > 0)
				{
					Error(iRet);
					return 0;
				}
				else if(iRet < 0)
				{
					switch(iRet)
					{
					case -1 :
						sprintf(C2F(cha1).buf, "%1.4E", dblRcond);
						Msgs(5,1);
						break;
					case -2 :
						Msgs(9, (int)dblRcond);
						break;
					default :
						break;
					}
				}
				LhsVar(1) = Rhs + 1;
				PutLhsVar();
			}
		}
		else
		{// Scalar / Matrix 
			int iErr		= 0;
			int iResultRows	= 0;
			int iResultCols	= 0;
			int iInc1		= 0;
			int iInc2		= 0;
			int iResultSize	= 0;

			if(iSize1 == 1)
			{
				iResultRows = iCols2;
				iResultCols = iRows2;
				iInc1		= 0;
				iInc2		= 1;
			}
			else
			{
				iResultRows = iRows1;
				iResultCols = iCols1;
				iInc1		= 1;
				iInc2		= 0;
			}

			if(iRows1 < 0 && iSize2 != 1)
			{
				Error(14);
				return 0;
			}

			iResultSize	= iResultRows * iResultCols;
			if(iGlobalComplex == 1)
				iAllocComplexMatrixOfDouble(Rhs + 1, iResultRows, iResultCols, &pReturnReal, &pReturnImg);
			else
				iAllocMatrixOfDouble(Rhs + 1, iResultRows, iResultCols, &pReturnReal);

			if(iComplex1 == 0 && iComplex2 == 0)
			{// Real1 \ Real2 -> Real2 / Real1
				iErr = iRightDivisionRealMatrixByRealMatrix(
					pReal1,						iInc1, 
					pReal2,						iInc2, 
					pReturnReal, 1, iResultSize);
			}
			else if(iComplex1 == 1 && iComplex2 == 0)
			{// Real \ Complex -> Complex / Real
				iErr = iRightDivisionComplexMatrixByRealMatrix(
					pReal1,			pImg1,		iInc1, 
					pReal2,						iInc2,
					pReturnReal,	pReturnImg, 1, iResultSize);
			}
			else if(iComplex1 == 0 && iComplex2 == 1)
			{// Complex \ Real -> Real / Complex
				iErr = iRightDivisionRealMatrixByComplexMatrix(
					pReal1,						iInc1, 
					pReal2,			pImg2,		iInc2, 
					pReturnReal,	pReturnImg,	1,  iResultSize);
			}
			else if(iComplex1 == 1 && iComplex2 == 1)
			{// Complex \ Complex
				iErr = iRightDivisionComplexMatrixByComplexMatrix(
					pReal1,			pImg1,		iInc1, 
					pReal2,			pImg2,		iInc2,
					pReturnReal,	pReturnImg,	1, iResultSize);
			}

			if(iErr != 0)
			{
				if(C2F(errgst).ieee == 0)
				{
					Error(27);
					return 0;
				}
				else if(C2F(errgst).ieee == 1)
				{
					Msgs(63,0);
				}
			}
			LhsVar(1) = Rhs + 1;
			PutLhsVar();
		}
	}
	return 0;
}
