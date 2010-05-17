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

int matldiv()
{
	int iRows1	= 0, iRows2	= 0, iCols1	= 0, iCols2	= 0;
	int iReal1	= 0, iReal2 = 0, iImg1	= 0, iImg2	= 0;
	int iSize1	= 0, iSize2 = 0;
	int iGlobalComplex		= 0;
	int iRet	= 0;
	int iIndex	= 0;

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
		if(iRows2 < 0)
		{
			Error(14);
			return 0;
		}

		if(iRows2 == 0)
		{
			LhsVar(1) = 2;
			PutLhsVar();
			return 0;
		}

		if(iRows1 != iRows2 && iSize2 != 1)
		{
			Error(265);
			return 0;
		}

		if(iRows1 == 0 || iCols1 == 0)
		{
			iAllocMatrixOfDouble(Rhs + 1, 0, 0, &pReturnReal);
			LhsVar(1) = Rhs + 1;
			PutLhsVar();
			return 0;
		}

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

		if(iRows1 * iCols1 != 1)
		{

			if(iComplex1 == 0 && iComplex2 == 0)
			{// A and B are both real
				double dblRcond = 0;
				iAllocMatrixOfDouble(Rhs + 1, iCols1, iCols2, &pReturnReal);
				iRet = iLeftDivisionOfRealMatrix(pReal1, iRows1, iCols1, pReal2, iRows2, iCols2, pReturnReal, iCols1, iCols2, &dblRcond);

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
				iAllocComplexMatrixOfDouble(Rhs + 1, iCols1, iCols2, &pReturnReal, &pReturnImg);
				iRet = iLeftDivisionOfComplexMatrix(
					pReal1, pImg1, iRows1, iCols1, 
					pReal2, pImg2, iRows2, iCols2, 
					pReturnReal, pReturnImg, iCols1, iCols2, &dblRcond);

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
		{// Real \ Matrix 
			int iErr = 0;
			if(iRows1 < 0 && iSize2 != 1)
			{
				Error(14);
				return 0;
			}
			
			if(iGlobalComplex)
			{
				iAllocComplexMatrixOfDouble(Rhs + 1, iRows2, iCols2, &pReturnReal, &pReturnImg);
			}
			else
			{
				iAllocMatrixOfDouble(Rhs + 1, iRows2, iCols2, &pReturnReal);
			}

			if(iComplex1 == 0 && iComplex2 == 0)
			{// Real1 \ Real2 -> Real2 / Real1
				iErr = iRightDivisionRealMatrixByRealMatrix(
					pReal2,						1, 
					pReal1,						0, 
					pReturnReal, 1, iSize2);
			}
			else if(iComplex1 == 0 && iComplex2 == 1)
			{// Real \ Complex -> Complex / Real
				iErr = iRightDivisionComplexMatrixByRealMatrix(
					pReal2,			pImg2,		1, 
					pReal1,						0,
					pReturnReal,	pReturnImg, 1, iSize2);
			}
			else if(iComplex1 == 1 && iComplex2 == 0)
			{// Complex \ Real -> Real / Complex
				iErr = iRightDivisionRealMatrixByComplexMatrix(
					pReal2,						1, 
					pReal1,			pImg1,		0, 
					pReturnReal,	pReturnImg,	1,  iSize2);
			}
			else if(iComplex1 == 1 && iComplex2 == 1)
			{// Complex \ Complex
				iErr = iRightDivisionComplexMatrixByComplexMatrix(
					pReal2,			pImg2,		1, 
					pReal1,			pImg1,		0,
					pReturnReal,	pReturnImg,	1, iSize2);
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

