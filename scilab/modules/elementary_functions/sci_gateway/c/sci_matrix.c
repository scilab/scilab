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
#include "sciprint.h"
#include "localization.h"

#define _NEW_TONIO_
#define MAX_INTERGER	2147483647

int matrix_double(int _iRowsRet, int _iColsRet);
int matrix_bsparse(int _iRowsRet, int _iColsRet);
int matrix_sparse(int _iRowsRet, int _iColsRet);
int matrix_poly(int _iRowsRet, int _iColsRet);

/*--------------------------------------------------------------------------*/
extern int C2F(intmatrix) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_scimatrix) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iRows1			= 0;
	int iCols1			= 0;
	int iRows2			= 0;
	int iCols2			= 0;
	int iRows3			= 0;
	int iCols3			= 0;
	int iRowsRet		= 0;
	int iColsRet		= 0;

	int iRealData1		= 0;
	int iImgData1		= 0;
	int iRealData2		= 0;
	int iRealData3		= 0;
	int iIndex			= 0;
	int iType			= 0;

	double *pdblRealData1	= NULL;
	double *pdblImgData1	= NULL;
	double *pdblRealData2	= NULL;
	double *pdblRealData3	= NULL;
	double* pReturnRealData	= NULL;
	double* pReturnImgData	= NULL;

	CheckRhs(2,1000000);
	CheckLhs(1,1);

	if(Rhs > 3)
	{//trouver un moyen d'appeller %hm_matrix :(
		return 0;
	}

	iType = GetType(1);

	if(	iType != sci_matrix && 
		iType != sci_sparse && 
		iType != sci_boolean_sparse && 
		iType != sci_poly &&
		iType != sci_ints)
	{
		OverLoad(1);
		return 0;
	}

	if(GetType(Rhs) != sci_matrix)
	{
		Error(53);
		return 0;
	}

	if(Rhs == 2)
	{
		if(iIsComplex(Rhs))
		{
			Error(32);
			return 0;
		}

		GetRhsVar(Rhs, MATRIX_OF_DOUBLE_DATATYPE, &iRows2, &iCols2, &iRealData2);
		pdblRealData2 = stk(iRealData2);

		if(iRows2 * iCols2 == 1)
		{
			iRowsRet = (int)pdblRealData2[0];
			iColsRet = 1;
		}
		else if(iRows2 * iCols2 == 2)
		{
			iRowsRet = (int)pdblRealData2[0];
			iColsRet = (int)pdblRealData2[1];
		}
		else
		{//trouver un moyen d'appeller %hm_matrix :(
			return 0;
		}
	}
	else
	{
		if(iIsComplex(3) || iIsComplex(2))
		{
			Error(32);
			return 0;
		}

		GetRhsVar(3, MATRIX_OF_DOUBLE_DATATYPE, &iRows3, &iCols3, &iRealData3);
		pdblRealData3 = stk(iRealData3);

		GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows2, &iCols2, &iRealData2);
		pdblRealData2 = stk(iRealData2);

		if(iRows3 * iCols3 != 1 || iRows2 * iCols2 != 1)
		{
			Error(52);
			return 0;
		}
		else
		{
			iColsRet = (int)pdblRealData3[0];
			iRowsRet = (int)pdblRealData2[0];
			if(iColsRet < -1 || iRowsRet < -1)
			{
				Error(116);
				return 0;
			}
		}
	}

	if(iColsRet == -1 && iRowsRet == -1)
	{
		Error(42);
		return 0;
	}

	GetVarDimension(1, &iRows1, &iCols1);
	CheckVarUsed(1);

	if(iRowsRet == -1)
		iRowsRet = (iRows1 * iCols1) / iColsRet;

	if(iColsRet == -1)
		iColsRet = (iRows1 * iCols1) / iRowsRet;

	if(iRowsRet * iColsRet != iRows1 * iCols1)
	{
		Error(60);
		return 0;
	}

	if(iRowsRet * iColsRet > MAX_INTERGER)
	{
		sciprint(_("%s: interger overflow.\n"), fname);
		Error(9999);
	}

	switch(iType)
	{
	case sci_boolean_sparse :
		matrix_bsparse(iRowsRet, iColsRet);
		break;
	case sci_sparse :
		matrix_sparse(iRowsRet, iColsRet);
		break;
	case sci_poly:
		matrix_poly(iRowsRet, iColsRet);
		break;
	default :
		matrix_double(iRowsRet, iColsRet);
		break;
	}

#else // _NEW_TONIO_
	C2F(intmatrix)(id);
#endif
	return 0;
}

int matrix_sparse(int _iRowsRet, int _iColsRet)
{
	int iRows			= 0;
	int iCols			= 0;
	int iRealData		= 0;
	int iImgData		= 0;
	int iTotalElem		= 0;
	int iTotalElemRet	= 0;
	int *piElemByRow	= NULL;
	int *piColByRow		= NULL;
	int *piElemByRowRet	= NULL;
	int *piColByRowRet	= NULL;
	int *piSaveiSum		= NULL;
	int iIndex			= 0;
	int iIndex2			= 0;
	int iSum			= 0;

	double *pdblRealData	= 0;
	double *pdblImgData		= 0;
	double *pReturnRealData	= NULL;
	double *pReturnImgData	= NULL;

	if(iIsComplex(1))
	{
		double *pOrder[3];
		GetRhsCSparseVar(1, &iRows, &iCols, &iTotalElem, NULL, NULL, &iRealData, &iImgData);
		piElemByRow	= (int*)malloc(iRows * sizeof(int));
		piColByRow	= (int*)malloc(iTotalElem * sizeof(int));
		GetRhsCSparseVar(1, &iRows, &iCols, &iTotalElem, piElemByRow, piColByRow, &iRealData, &iImgData);

		pdblRealData	= stk(iRealData);
		pdblImgData		= stk(iImgData);

		//pReturnRealData	= (double*)malloc(iTotalElem * sizeof(double));
		//pReturnImgData	= (double*)malloc(iTotalElem * sizeof(double));
		pOrder[0] = (double*)malloc(iTotalElem * sizeof(double));
		pOrder[1] = (double*)malloc(iTotalElem * sizeof(double));
		pOrder[2] = (double*)malloc(iTotalElem * sizeof(double));

		//piElemByRowRet	=	(int*)malloc(_iRowsRet * sizeof(int));
		//piColByRowRet	=	(int*)malloc(iTotalElem * sizeof(int));

		iAllocComplexSparseMatrix(Rhs + 1, 1, _iRowsRet, _iColsRet, iTotalElem , &piElemByRowRet, &piColByRowRet, &pReturnRealData, &pReturnImgData);
		memset(piElemByRowRet, 0x00, _iRowsRet * sizeof(int));

		iSum = 0;
		for(iIndex = 0 ; iIndex < iRows ; iIndex++)
		{
			for(iIndex2 = 0 ; iIndex2 < piElemByRow[iIndex] ; iIndex2++)
			{
				iSum = iArraySum(piElemByRow,-1,iIndex);
				pOrder[0][iTotalElemRet] = (piColByRow[iSum + iIndex2]-1) * iRows + iIndex;
				pOrder[1][iTotalElemRet] = pdblRealData[iTotalElemRet];
				iTotalElemRet++;
			}
		}

		for(iIndex = 0 ; iIndex < iTotalElem ; iIndex++)
		{
			for(iIndex2 = 1 ; iIndex2 < iTotalElem - iIndex ; iIndex2++)
			{
				if(pOrder[0][iIndex2] < pOrder[0][iIndex2 - 1])
				{
					double dblTemp			= pOrder[0][iIndex2 - 1];
					pOrder[0][iIndex2 - 1]	= pOrder[0][iIndex2];
					pOrder[0][iIndex2]		= dblTemp;

					dblTemp					= pOrder[1][iIndex2 - 1];
					pOrder[1][iIndex2 - 1]	= pOrder[1][iIndex2];
					pOrder[1][iIndex2]		= dblTemp;
				}
			}
		}

		for(iIndex = 0 ; iIndex < iTotalElem ; iIndex++)
		{
			pOrder[2][iIndex] = (int)pOrder[0][iIndex] / _iRowsRet;
			pOrder[0][iIndex] = (int)pOrder[0][iIndex] % _iRowsRet;
		}

		iTotalElemRet = 0;
		for(iIndex = 0 ; iIndex < _iRowsRet ; iIndex++)
		{
			for(iIndex2 = 0 ; iIndex2 < iTotalElem ; iIndex2++)
			{
				if(pOrder[0][iIndex2] == iIndex)
				{
					piElemByRowRet[iIndex]++;
					piColByRowRet[iTotalElemRet] = (int)pOrder[2][iIndex2] + 1;
					pReturnRealData[iTotalElemRet] = pOrder[1][iIndex2];
					pReturnImgData[iTotalElemRet] = pOrder[1][iIndex2];
					iTotalElemRet++;
				}
			}
		}

		//CreateSparseVarFromPtr(Rhs + 1, _iRowsRet, _iColsRet, iTotalElem, piElemByRowRet, piColByRowRet, pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();

		free(pOrder[0]);
		free(pOrder[1]);
		free(pOrder[2]);
		//free(piElemByRowRet);
		//free(piColByRowRet);
		free(piElemByRow);
		free(piColByRow);
		//free(pReturnRealData);
		//free(pReturnImgData);
	}
	else
	{//void GetRhsSparseVar(int _iVarNum, int* _piRows, int* _piCols, int* _piRowsElem, int* _piColsElem, int* _piReal);
		double *pOrder[3];
		GetRhsSparseVar(1, &iRows, &iCols, &iTotalElem, NULL, NULL, &iRealData);
		piElemByRow	= (int*)malloc(iRows * sizeof(int));
		piColByRow	= (int*)malloc(iTotalElem * sizeof(int));
		GetRhsSparseVar(1, &iRows, &iCols, &iTotalElem, piElemByRow, piColByRow, &iRealData);

		pdblRealData	= stk(iRealData);

		pReturnRealData	= (double*)malloc(iTotalElem * sizeof(double));
		pOrder[0] = (double*)malloc(iTotalElem * sizeof(double));
		pOrder[1] = (double*)malloc(iTotalElem * sizeof(double));
		pOrder[2] = (double*)malloc(iTotalElem * sizeof(double));

		//piElemByRowRet	=	(int*)malloc(_iRowsRet * sizeof(int));
		//piColByRowRet	=	(int*)malloc(iTotalElem * sizeof(int));

		iAllocSparseMatrix(Rhs + 1, _iRowsRet, _iColsRet, iTotalElem , &piElemByRowRet, &piColByRowRet, &pReturnRealData);
		memset(piElemByRowRet, 0x00, _iRowsRet * sizeof(int));

		iSum = 0;
		for(iIndex = 0 ; iIndex < iRows ; iIndex++)
		{
			for(iIndex2 = 0 ; iIndex2 < piElemByRow[iIndex] ; iIndex2++)
			{
				iSum = iArraySum(piElemByRow,-1,iIndex);
				pOrder[0][iTotalElemRet] = (piColByRow[iSum + iIndex2]-1) * iRows + iIndex;
				pOrder[1][iTotalElemRet] = pdblRealData[iTotalElemRet];
				iTotalElemRet++;
			}
		}

		for(iIndex = 0 ; iIndex < iTotalElem ; iIndex++)
		{
			for(iIndex2 = 1 ; iIndex2 < iTotalElem - iIndex ; iIndex2++)
			{
				if(pOrder[0][iIndex2] < pOrder[0][iIndex2 - 1])
				{
					double dblTemp			= pOrder[0][iIndex2 - 1];
					pOrder[0][iIndex2 - 1]	= pOrder[0][iIndex2];
					pOrder[0][iIndex2]		= dblTemp;

					dblTemp					= pOrder[1][iIndex2 - 1];
					pOrder[1][iIndex2 - 1]	= pOrder[1][iIndex2];
					pOrder[1][iIndex2]		= dblTemp;
				}
			}
		}

		for(iIndex = 0 ; iIndex < iTotalElem ; iIndex++)
		{
			pOrder[2][iIndex] = (int)pOrder[0][iIndex] / _iRowsRet;
			pOrder[0][iIndex] = (int)pOrder[0][iIndex] % _iRowsRet;
		}

		iTotalElemRet = 0;
		for(iIndex = 0 ; iIndex < _iRowsRet ; iIndex++)
		{
			for(iIndex2 = 0 ; iIndex2 < iTotalElem ; iIndex2++)
			{
				if(pOrder[0][iIndex2] == iIndex)
				{
					piElemByRowRet[iIndex]++;
					piColByRowRet[iTotalElemRet] = (int)pOrder[2][iIndex2] + 1;
					pReturnRealData[iTotalElemRet] = pOrder[1][iIndex2];
					iTotalElemRet++;
				}
			}
		}

		//CreateSparseVarFromPtr(Rhs + 1, _iRowsRet, _iColsRet, iTotalElem, piElemByRowRet, piColByRowRet, pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();

		free(pOrder[0]);
		free(pOrder[1]);
		free(pOrder[2]);
		//free(piElemByRowRet);
		//free(piColByRowRet);
		free(piElemByRow);
		free(piColByRow);
		//free(pReturnRealData);
	}
	return 0;
}

int matrix_bsparse(int _iRowsRet, int _iColsRet)
{
	int iRows			= 0;
	int iCols			= 0;
	int iRealData		= 0;
	int iImgData		= 0;
	int iTotalElem		= 0;
	int iTotalElemRet	= 0;
	int *piElemByRow	= NULL;
	int *piColByRow		= NULL;
	int *piElemByRowRet	= NULL;
	int *piColByRowRet	= NULL;
	int *piSaveiSum		= NULL;
	int iIndex			= 0;
	int iIndex2			= 0;
	int iSum			= 0;


	int *pOrder[3];
	GetRhsBooleanSparseVar(1, &iRows, &iCols, &iTotalElem, NULL, NULL);
	piElemByRow	= (int*)malloc(iRows * sizeof(int));
	piColByRow	= (int*)malloc(iTotalElem * sizeof(int));
	GetRhsBooleanSparseVar(1, &iRows, &iCols, &iTotalElem, piElemByRow, piColByRow);

	iAllocBooleanSparseMatrix(Rhs + 1, _iRowsRet, _iColsRet, iTotalElem, &piElemByRowRet, &piColByRowRet);
	pOrder[0] = (int*)malloc(iTotalElem * sizeof(int));
	pOrder[2] = (int*)malloc(iTotalElem * sizeof(int));

	//piElemByRowRet	=	(int*)malloc(_iRowsRet * sizeof(int));
	//piColByRowRet	=	(int*)malloc(iTotalElem * sizeof(int));
	memset(piElemByRowRet, 0x00, _iRowsRet * sizeof(int));

	iSum = 0;
	for(iIndex = 0 ; iIndex < iRows ; iIndex++)
	{
		for(iIndex2 = 0 ; iIndex2 < piElemByRow[iIndex] ; iIndex2++)
		{
			iSum = iArraySum(piElemByRow,-1,iIndex);
			pOrder[0][iTotalElemRet] = (piColByRow[iSum + iIndex2]-1) * iRows + iIndex;
			iTotalElemRet++;
		}
	}

	for(iIndex = 0 ; iIndex < iTotalElem ; iIndex++)
	{
		for(iIndex2 = 1 ; iIndex2 < iTotalElem - iIndex ; iIndex2++)
		{
			if(pOrder[0][iIndex2] < pOrder[0][iIndex2 - 1])
			{
				int iTemp				= pOrder[0][iIndex2 - 1];
				pOrder[0][iIndex2 - 1]	= pOrder[0][iIndex2];
				pOrder[0][iIndex2]		= iTemp;
			}
		}
	}

	for(iIndex = 0 ; iIndex < iTotalElem ; iIndex++)
	{
		pOrder[2][iIndex] = pOrder[0][iIndex] / _iRowsRet;
		pOrder[0][iIndex] = pOrder[0][iIndex] % _iRowsRet;
	}

	iTotalElemRet = 0;
	for(iIndex = 0 ; iIndex < _iRowsRet ; iIndex++)
	{
		for(iIndex2 = 0 ; iIndex2 < iTotalElem ; iIndex2++)
		{
			if(pOrder[0][iIndex2] == iIndex)
			{
				piElemByRowRet[iIndex]++;
				piColByRowRet[iTotalElemRet]	= pOrder[2][iIndex2] + 1;
				iTotalElemRet++;
			}
		}
	}

	//CreateCBooleanSparseVarFromPtr(Rhs + 1, _iRowsRet, _iColsRet, iTotalElem, piElemByRowRet, piColByRowRet);
	LhsVar(1) = Rhs + 1;
	PutLhsVar();

	free(pOrder[0]);
	free(pOrder[2]);
	//free(piElemByRowRet);
	//free(piColByRowRet);
	free(piElemByRow);
	free(piColByRow);
	return 0;
}

int matrix_double(int _iRowsRet, int _iColsRet)
{
	int iRows = 0;
	int iCols = 0;
	int iRealData = 0;
	int iImgData = 0;
	int iIndex = 0;

	if(iIsComplex(1))
	{
		double *pdblRealData	= 0;
		double *pdblImgData		= 0;
		int		iComplex		= 1;

		GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iRealData, &iImgData);
		pdblRealData	= stk(iRealData);
		pdblImgData		= stk(iImgData);

		CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &_iRowsRet, &_iColsRet, &pdblRealData, &pdblImgData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
	}
	else
	{
		double *pdblRealData	= 0;
		int		itr				= 0;

		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		pdblRealData		= stk(iRealData);

		CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &_iRowsRet, &_iColsRet, &pdblRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
	}
	return 0;
}

int matrix_poly(int _iRowsRet, int _iColsRet)
{
	int iRows = 0;
	int iCols = 0;
	int iRealData = 0;
	int iImgData = 0;
	int iIndex = 0;
	int *piPow = NULL;
	int *piVarName = NULL;
	int iMaxData = 0;

	double *pdblRealData	= 0;
	double *pdblImgData		= 0;
	double *pReturnRealData	= NULL;
	double *pReturnImgData	= NULL;

	if(iIsComplex(1))
	{
		GetRhsCPolyVar(1, &piVarName, &iRows, &iCols, NULL, &iRealData, &iImgData);
		piPow				= (int*)malloc(iRows * iCols * sizeof(int));
		GetRhsCPolyVar(1, &piVarName, &iRows, &iCols, piPow, &iRealData, &iImgData);
		iMaxData			= iArraySum(piPow, 0, iRows * iCols);

		pdblRealData		= stk(iRealData);
		pdblImgData			= stk(iImgData);

		CreateCPolyVarFromPtr(Rhs + 1, &piVarName, _iRowsRet, _iColsRet, piPow, pdblRealData, pdblImgData);

	}
	else
	{
		GetRhsPolyVar(1, &piVarName, &iRows, &iCols, NULL, &iRealData);
		piPow	= (int*)malloc(iRows * iCols * sizeof(int));
		GetRhsPolyVar(1, &piVarName, &iRows, &iCols, piPow, &iRealData);
		iMaxData = iArraySum(piPow, 0, iRows * iCols);

		pdblRealData	= stk(iRealData);

		CreatePolyVarFromPtr(Rhs + 1, &piVarName, _iRowsRet, _iColsRet, piPow, pdblRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
	}
	return 0;
}

/*--------------------------------------------------------------------------*/
