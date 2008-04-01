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

int real_poly();
int real_sparse();
int real_double();

/*--------------------------------------------------------------------------*/
extern int C2F(intreal) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_real) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
	int iRows = 0;
	int iCols = 0;
	int iRealData = 0;
	int iImgData = 0;
	int iMaxData = 0;
	
	CheckRhs(1,1);
	CheckLhs(1,1);

	switch(GetType(1))
	{
	case sci_matrix :
		real_double();
		break;
	case sci_poly :
		real_poly();
		break;
	case sci_sparse :
		real_sparse();
		break;
	default:
		OverLoad(1);
		break;
	}
	return 0;
}

int real_poly()
{
	int		iRows			= 0;
	int		iCols			= 0;
	int		iRealData		= 0;
	int		iMaxData		= 0;
	double	*pdblRealData	= 0;
	double	*pdblImgData	= 0;
	double	*pReturnRealData= NULL;
	double	*pReturnImgData	= NULL;

	int *piVarName = NULL;
	int *piPow = NULL;
	GetRhsPolyVar(1, &piVarName, &iRows, &iCols, NULL, &iRealData);

	piPow				= (int*)malloc(iRows * iCols * sizeof(int));
	GetRhsPolyVar(1, &piVarName, &iRows, &iCols, piPow, &iRealData);

	iMaxData			= iArraySum(piPow, 0, iRows * iCols);
	pdblRealData		= stk(iRealData);
	pReturnRealData		= (double*)malloc(iMaxData * sizeof(double));

	memcpy(pReturnRealData, pdblRealData, iMaxData * sizeof(double));
	CreatePolyVarFromPtr(Rhs + 1, &piVarName, iRows, iCols, piPow, pReturnRealData);

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	free(pReturnRealData);
	return 0;
}

int real_sparse()
{
	int iRows			= 0;
	int iCols			= 0;
	int iRealData		= 0;
	int iImgData		= 0;
	int iTotalElem		= 0;
	int *piElemByRow	= NULL;
	int *piColByRow		= NULL;
	int iDimChange		= 0;

	int iIndex = 0, iIndex2 = 0, iIndex3 = 0, iIndex4 = 0;

	double *pdblRealData	= 0;
	double *pdblImgData		= 0;
	double *pReturnRealData	= NULL;
	double *pReturnImgData	= NULL;

	GetRhsSparseVar(1, &iRows, &iCols, &iTotalElem, NULL, NULL, &iRealData);
	piElemByRow	= (int*)malloc(iRows * sizeof(int));
	piColByRow	= (int*)malloc(iTotalElem * sizeof(int));
	GetRhsSparseVar(1, &iRows, &iCols, &iTotalElem, piElemByRow, piColByRow, &iRealData);

	pdblRealData	= stk(iRealData);
	pReturnRealData	= (double*)malloc(iTotalElem * sizeof(double));

	//special case, when a value is a pure imaginary number, in this case
	//the dimaension of the spase stay the same but 
	//iTotalElem piElemByRow, piColByRow must change...

	for(iIndex = 0 ; iIndex < iRows ; iIndex++)
	{
		for(iIndex2 = 0 ; iIndex2 < piElemByRow[iIndex] ; iIndex2++)
		{
			pReturnRealData[iIndex3] = pdblRealData[iIndex4];
			if(pReturnRealData[iIndex3] == 0)
			{
				//special case, when a value is a pure imaginary number, in this case
				//the dimension of the spase stay the same but 
				//iTotalElem piElemByRow, piColByRow must change...
				piElemByRow[iIndex]--;
				piColByRow[iIndex]	= 0;
				iDimChange			= 1;
			}
			else//only if real part is non null
				iIndex3++;
			iIndex4++;
		}
	}

	if(iDimChange == 1)
	{
		for(iIndex = 0, iIndex2 = 0 ; iIndex < iTotalElem ; iIndex++)
		{
			if(piColByRow[iIndex] == 0)
				continue;
			piColByRow[iIndex2] = piColByRow[iIndex];
			iIndex2++;
		}
		iTotalElem = iArraySum(piElemByRow, 0 , iRows);
	}

	CreateSparseVarFromPtr(Rhs + 1, iRows, iCols, iTotalElem, piElemByRow, piColByRow, pReturnRealData);
	LhsVar(1) = Rhs + 1;
	PutLhsVar();

	free(piElemByRow);
	free(piColByRow);
	free(pReturnRealData);

	return 0;
}

int real_double()
{
	int iRows = 0;
	int iCols = 0;
	int iRealData = 0;
	double *pdblRealData	= 0;
	double *pReturnRealData	= NULL;
	int		iComplex		= 1;

	GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);

	pdblRealData	= stk(iRealData);
	pReturnRealData = (double*)malloc(iRows * iCols * sizeof(double));

	memcpy(pReturnRealData, pdblRealData, iRows * iCols * sizeof(double));

	CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	free(pReturnRealData);
	return 0;
}
/*--------------------------------------------------------------------------*/
