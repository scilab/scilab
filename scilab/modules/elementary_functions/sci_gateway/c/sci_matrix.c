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
#include "api_scilab.h"
#include "Scierror.h"

#define MAX_INTERGER	2147483647

int matrix_double(int* _piAddress, int _iRowsRet, int _iColsRet);
int matrix_bsparse(int* _piAddress, int _iRowsRet, int _iColsRet);
int matrix_sparse(int* _piAddress, int _iRowsRet, int _iColsRet);
int matrix_poly(int* _piAddress, int _iRowsRet, int _iColsRet);
int	matrix_string(int* _piAddress, int _iRowsRet, int _iColsRet);
int matrix_int(int* _piAddress, int _iRowsRet, int _iColsRet);
int	matrix_bool(int* _piAddress, int _iRowsRet, int _iColsRet);

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
int C2F(sci_scimatrix) (char *fname,unsigned long fname_len)
{
	int iRet				= 0;
	int iRows1			= 0;
	int iCols1			= 0;
	int iRows2			= 0;
	int iCols2			= 0;
	int iRows3			= 0;
	int iCols3			= 0;
	int iRowsRet		= 0;
	int iColsRet		= 0;

	int* piAddr1		= NULL;
	int* piAddr2		= NULL;
	int* piAddr3		= NULL;

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
	{//call %hm_matrix
		int iStart	= 1;
		int iRhs		= Rhs;
		int iLhs		= Lhs;

		SciString(&iStart,"%hm_matrix", &iLhs, &iRhs);

		LhsVar(1) = 1;
		PutLhsVar();
		return 0;
	}

	getVarAddressFromPosition(1, &piAddr1);

	iType = getVarType(piAddr1);

	if(	iType != sci_matrix && 
		iType != sci_boolean && 
		iType != sci_sparse && 
		iType != sci_boolean_sparse && 
		iType != sci_poly &&
		iType != sci_strings &&
		iType != sci_ints)
	{
		OverLoad(1);
		return 0;
	}

	if(Rhs == 2)
	{
		getVarAddressFromPosition(2, &piAddr2);
		if(isVarComplex(piAddr2))
		{
			Error(32);
			return 0;
		}

		getMatrixOfDouble(piAddr2, &iRows2, &iCols2, &pdblRealData2);

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
		getVarAddressFromPosition(2, &piAddr2);
		getVarAddressFromPosition(3, &piAddr3);
		if(isVarComplex(piAddr3) || isVarComplex(piAddr2))
		{
			Error(32);
			return 0;
		}

		getMatrixOfDouble(piAddr3, &iRows3, &iCols3, &pdblRealData3);
		getMatrixOfDouble(piAddr2, &iRows2, &iCols2, &pdblRealData2);


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

	getVarDimension(piAddr1, &iRows1, &iCols1);

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
	case sci_matrix : 
		iRet = matrix_double(piAddr1, iRowsRet, iColsRet);
		break;
	case sci_poly:
		matrix_poly(piAddr1, iRowsRet, iColsRet);
		break;
	case sci_boolean :
		matrix_bool(piAddr1, iRowsRet, iColsRet);
		break;
	case sci_sparse :
		matrix_sparse(piAddr1, iRowsRet, iColsRet);
		break;
	case sci_boolean_sparse :
		matrix_bsparse(piAddr1, iRowsRet, iColsRet);
		break;
	case sci_ints:
		matrix_int(piAddr1, iRowsRet, iColsRet);
		break;
	case sci_strings:
		matrix_string(piAddr1, iRowsRet, iColsRet);
		break;
	default :
		return 1;
		break;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();

	return 0;
}

int matrix_sparse(int* _piAddress, int _iRowsRet, int _iColsRet)
{
	int iRet						= 0;
	int iRows						= 0;
	int iCols						= 0;
	int iNbItem					= 0;
	int iNbItemRet			= 0;
	int *piNbItemRow		= NULL;
	int *piColPos				= NULL;
	int *piNbItemRowRet	= NULL;
	int *piColPosRet		= NULL;
	int *piSaveiSum			= NULL;
	int iIndex					= 0;
	int iIndex2					= 0;
	int iSum						= 0;

	double *pdblReal		= 0;
	double *pdblImg			= 0;
	double *pdblRealRet	= NULL;
	double *pdblImgRet	= NULL;

	if(isVarComplex(_piAddress))
	{
		double *pOrder[3];
		iRet = getComplexSparseMatrix(_piAddress, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
		if(iRet)
		{
			return 1;
		}

		pOrder[0] = (double*)malloc(iNbItem * sizeof(double));
		pOrder[1] = (double*)malloc(iNbItem * sizeof(double));
		pOrder[2] = (double*)malloc(iNbItem * sizeof(double));

		iRet = allocComplexSparseMatrix(Rhs + 1, _iRowsRet, _iColsRet, iNbItem, &piNbItemRowRet, &piColPosRet, &pdblRealRet, &pdblImgRet);
		if(iRet)
		{
			return 1;
		}

		memset(piNbItemRowRet, 0x00, _iRowsRet * sizeof(int));

		iSum = 0;
		for(iIndex = 0 ; iIndex < iRows ; iIndex++)
		{
			for(iIndex2 = 0 ; iIndex2 < piNbItemRow[iIndex] ; iIndex2++)
			{
				iSum = iArraySum(piNbItemRow,-1,iIndex);
				pOrder[0][iNbItemRet] = (piColPos[iSum + iIndex2]-1) * iRows + iIndex;
				pOrder[1][iNbItemRet] = pdblReal[iNbItemRet];
				iNbItemRet++;
			}
		}

		for(iIndex = 0 ; iIndex < iNbItem ; iIndex++)
		{
			for(iIndex2 = 1 ; iIndex2 < iNbItem - iIndex ; iIndex2++)
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

		for(iIndex = 0 ; iIndex < iNbItem ; iIndex++)
		{
			pOrder[2][iIndex] = (int)pOrder[0][iIndex] / _iRowsRet;
			pOrder[0][iIndex] = (int)pOrder[0][iIndex] % _iRowsRet;
		}

		iNbItemRet = 0;
		for(iIndex = 0 ; iIndex < _iRowsRet ; iIndex++)
		{
			for(iIndex2 = 0 ; iIndex2 < iNbItem ; iIndex2++)
			{
				if(pOrder[0][iIndex2] == iIndex)
				{
					piNbItemRowRet[iIndex]++;
					piColPosRet[iNbItemRet] = (int)pOrder[2][iIndex2] + 1;
					pdblRealRet[iNbItemRet] = pOrder[1][iIndex2];
					pdblImgRet[iNbItemRet]	= pOrder[1][iIndex2];
					iNbItemRet++;
				}
			}
		}

		free(pOrder[0]);
		free(pOrder[1]);
		free(pOrder[2]);
	}
	else
	{//void GetRhsSparseVar(int _iVarNum, int* _piRows, int* _piCols, int* _piRowsElem, int* _piColsElem, int* _piReal);
		double *pOrder[3];
		iRet = getSparseMatrix(_piAddress, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal);
		if(iRet)
		{
			return 1;
		}

		pOrder[0] = (double*)malloc(iNbItem * sizeof(double));
		pOrder[1] = (double*)malloc(iNbItem * sizeof(double));
		pOrder[2] = (double*)malloc(iNbItem * sizeof(double));

		iRet = allocSparseMatrix(Rhs + 1, _iRowsRet, _iColsRet, iNbItem, &piNbItemRowRet, &piColPosRet, &pdblRealRet);
		if(iRet)
		{
			return 1;
		}
		memset(piNbItemRowRet, 0x00, _iRowsRet * sizeof(int));

		iSum = 0;
		for(iIndex = 0 ; iIndex < iRows ; iIndex++)
		{
			for(iIndex2 = 0 ; iIndex2 < piNbItemRow[iIndex] ; iIndex2++)
			{
				iSum = iArraySum(piNbItemRow,-1,iIndex);
				pOrder[0][iNbItemRet] = (piColPos[iSum + iIndex2]-1) * iRows + iIndex;
				pOrder[1][iNbItemRet] = pdblReal[iNbItemRet];
				iNbItemRet++;
			}
		}

		for(iIndex = 0 ; iIndex < iNbItem ; iIndex++)
		{
			for(iIndex2 = 1 ; iIndex2 < iNbItem - iIndex ; iIndex2++)
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

		for(iIndex = 0 ; iIndex < iNbItem ; iIndex++)
		{
			pOrder[2][iIndex] = (int)pOrder[0][iIndex] / _iRowsRet;
			pOrder[0][iIndex] = (int)pOrder[0][iIndex] % _iRowsRet;
		}

		iNbItemRet = 0;
		for(iIndex = 0 ; iIndex < _iRowsRet ; iIndex++)
		{
			for(iIndex2 = 0 ; iIndex2 < iNbItem ; iIndex2++)
			{
				if(pOrder[0][iIndex2] == iIndex)
				{
					piNbItemRowRet[iIndex]++;
					piColPosRet[iNbItemRet] = (int)pOrder[2][iIndex2] + 1;
					pdblRealRet[iNbItemRet] = pOrder[1][iIndex2];
					iNbItemRet++;
				}
			}
		}

		free(pOrder[0]);
		free(pOrder[1]);
		free(pOrder[2]);
	}
	return 0;
}

int matrix_bsparse(int* _piAddress, int _iRowsRet, int _iColsRet)
{
	int iRet						= 0;
	int iRows						= 0;
	int iCols						= 0;
	int iNbItem					= 0;
	int iNbItemRet			= 0;
	int* piNbItemRow		= NULL;
	int *piColPos				= NULL;
	int *piNbItemRowRet	= NULL;
	int *piColPosRet		= NULL;
	int *piSaveiSum			= NULL;
	int iIndex					= 0;
	int iIndex2					= 0;
	int iSum						= 0;


	int *pOrder[3];
	iRet = getBooleanSparseMatrix(_piAddress, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos);
	if(iRet)
	{
		return 1;
	}

	pOrder[0] = (int*)malloc(iNbItem * sizeof(int));
	pOrder[2] = (int*)malloc(iNbItem * sizeof(int));

	iRet = allocBooleanSparseMatrix(Rhs + 1, _iRowsRet, _iColsRet, iNbItem, &piNbItemRowRet, &piColPosRet);
	if(iRet)
	{
		return 1;
	}

	memset(piNbItemRowRet, 0x00, _iRowsRet * sizeof(int));

	iSum = 0;
	for(iIndex = 0 ; iIndex < iRows ; iIndex++)
	{
		for(iIndex2 = 0 ; iIndex2 < piNbItemRow[iIndex] ; iIndex2++)
		{
			iSum = iArraySum(piNbItemRow,-1,iIndex);
			pOrder[0][iNbItemRet] = (piColPos[iSum + iIndex2]-1) * iRows + iIndex;
			iNbItemRet++;
		}
	}

	for(iIndex = 0 ; iIndex < iNbItem ; iIndex++)
	{
		for(iIndex2 = 1 ; iIndex2 < iNbItem - iIndex ; iIndex2++)
		{
			if(pOrder[0][iIndex2] < pOrder[0][iIndex2 - 1])
			{
				int iTemp				= pOrder[0][iIndex2 - 1];
				pOrder[0][iIndex2 - 1]	= pOrder[0][iIndex2];
				pOrder[0][iIndex2]		= iTemp;
			}
		}
	}

	for(iIndex = 0 ; iIndex < iNbItem ; iIndex++)
	{
		pOrder[2][iIndex] = pOrder[0][iIndex] / _iRowsRet;
		pOrder[0][iIndex] = pOrder[0][iIndex] % _iRowsRet;
	}

	iNbItemRet = 0;
	for(iIndex = 0 ; iIndex < _iRowsRet ; iIndex++)
	{
		for(iIndex2 = 0 ; iIndex2 < iNbItem ; iIndex2++)
		{
			if(pOrder[0][iIndex2] == iIndex)
			{
				piNbItemRowRet[iIndex]++;
				piColPosRet[iNbItemRet]	= pOrder[2][iIndex2] + 1;
				iNbItemRet++;
			}
		}
	}

	free(pOrder[0]);
	free(pOrder[2]);
	return 0;
}

int matrix_double(int* _piAddress, int _iRowsRet, int _iColsRet)
{
	int iRet					= 0;
	int iIndex				= 0;
	int iRows					= 0;
	int iCols					= 0;

	double* pdblReal	= NULL;
	double* pdblImg		= NULL;

	if(isVarComplex(_piAddress))
	{
		iRet = getComplexMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
		if(iRet)
		{
			return 1;
		}

		iRet = createComplexMatrixOfDouble(Rhs + 1, _iRowsRet, _iColsRet, pdblReal, pdblImg);
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

		iRet = createMatrixOfDouble(Rhs + 1, _iRowsRet, _iColsRet, pdblReal);
		if(iRet)
		{
			return 1;
		}
	}
	return 0;
}

int matrix_poly(int* _piAddress, int _iRowsRet, int _iColsRet)
{
	int i;
	int iRet					= 0;
	int iRows					= 0;
	int iCols					= 0;
	int iMaxData			= 0;
	int iLen					= 0;
	int* piCoeff			= NULL;
	double **pdblReal	= NULL;
	double **pdblImg	= NULL;
	char pstVarName[16];

	iRet = getPolyVariableName(_piAddress, pstVarName, &iLen);
	if(iRet)
	{
		return 1;
	}

	if(isVarComplex(_piAddress))
	{
		iRet		= getComplexMatrixOfPoly(_piAddress, &iRows, &iCols, NULL, NULL, NULL);
		if(iRet)
		{
			return 1;
		}

		piCoeff	= (int*)malloc(sizeof(int) * iRows * iCols);
		iRet		= getComplexMatrixOfPoly(_piAddress, &iRows, &iCols, piCoeff, NULL, NULL);
		if(iRet)
		{
			return 1;
		}

		pdblReal	= malloc(sizeof(double*) * iRows * iCols);
		pdblImg		= malloc(sizeof(double*) * iRows * iCols);
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblReal[i] = malloc(sizeof(double) * piCoeff[i]);
			pdblImg[i]	= malloc(sizeof(double) * piCoeff[i]);
		}

		iRet			= getComplexMatrixOfPoly(_piAddress, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
		if(iRet)
		{
			return 1;
		}

		iRet			= createComplexMatrixOfPoly(Rhs + 1, pstVarName, _iRowsRet, _iColsRet, piCoeff, pdblReal, pdblImg);
	}
	else
	{
		iRet		= getMatrixOfPoly(_piAddress, &iRows, &iCols, NULL, NULL);
		if(iRet)
		{
			return 1;
		}

		piCoeff	= (int*)malloc(sizeof(int) * iRows * iCols);
		iRet		= getMatrixOfPoly(_piAddress, &iRows, &iCols, piCoeff, NULL);
		if(iRet)
		{
			return 1;
		}

		pdblReal	= malloc(sizeof(double*) * iRows * iCols);
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblReal[i] = malloc(sizeof(double) * piCoeff[i]);
		}

		iRet			= getMatrixOfPoly(_piAddress, &iRows, &iCols, piCoeff, pdblReal);
		if(iRet)
		{
			return 1;
		}

		iRet			= createMatrixOfPoly(Rhs + 1, pstVarName, _iRowsRet, _iColsRet, piCoeff, pdblReal);
	}

	if(iRet)
	{
		return 1;
	}

	//delete
	free(piCoeff);

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		free(pdblReal[i]);
	}
	free(pdblReal);

	if(isVarComplex(_piAddress))
	{
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			free(pdblImg[i]);
		}
		free(pdblImg);
	}

	return 0;
}

int	matrix_string(int* _piAddress, int _iRowsRet, int _iColsRet)
{
	int i;
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int *piLen			= NULL;
	char **pstData	= NULL;

	iRet = getMatrixOfString(_piAddress, &iRows, &iCols, NULL, NULL);
	if(iRet)
	{
		return 1;
	}

	piLen = malloc(sizeof(int) * iRows * iCols);
	iRet = getMatrixOfString(_piAddress, &iRows, &iCols, piLen, NULL);
	if(iRet)
	{
		return 1;
	}

	pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+1 for null termination
	}

	iRet = getMatrixOfString(_piAddress, &iRows, &iCols, piLen, pstData);
	if(iRet)
	{
		return 1;
	}

	iRet = createMatrixOfString(Rhs + 1, _iRowsRet, _iColsRet, pstData);
	if(iRet)
	{
		return 1;
	}

	free(piLen);
	for(i = 0 ; i < iRows * iCols ; i++)
	{
		free(pstData[i]);
	}
	free(pstData);
	return 0;
}

int matrix_int(int* _piAddress, int _iRowsRet, int _iColsRet)
{
	int iRet								= 0;
	int iPrec								= 0;
	int iRows								= 0;
	int iCols								= 0;

	char* pcData						= NULL;
	short* psData						= NULL;
	int* piData							= NULL;
	unsigned char* pucData	= NULL;
	unsigned short* pusData	= NULL;
	unsigned int* puiData		= NULL;

	iRet = getMatrixOfIntegerPrecision(_piAddress, &iPrec);
	if(iRet)
	{
		return 1;
	}

	switch(iPrec)
	{
	case SCI_INT8 :
		iRet = getMatrixOfInteger8(_piAddress, &iRows, &iCols, &pcData);
		if(iRet)
		{
			return 1;
		}
		iRet = createMatrixOfInteger8(Rhs + 1, _iRowsRet, _iColsRet, pcData);
		break;
	case SCI_UINT8 :
		iRet = getMatrixOfUnsignedInteger8(_piAddress, &iRows, &iCols, &pucData);
		if(iRet)
		{
			return 1;
		}
		iRet = createMatrixOfUnsignedInteger8(Rhs + 1, _iRowsRet, _iColsRet, pucData);
		break;
	case SCI_INT16 :
		iRet = getMatrixOfInteger16(_piAddress, &iRows, &iCols, &psData);
		if(iRet)
		{
			return 1;
		}
		iRet = createMatrixOfInteger16(Rhs + 1, _iRowsRet, _iColsRet, psData);
		break;
	case SCI_UINT16 :
		iRet = getMatrixOfUnsignedInteger16(_piAddress, &iRows, &iCols, &pusData);
		if(iRet)
		{
			return 1;
		}
		iRet = createMatrixOfUnsignedInteger16(Rhs + 1, _iRowsRet, _iColsRet, pusData);
		break;
	case SCI_INT32 :
		iRet = getMatrixOfInteger32(_piAddress, &iRows, &iCols, &piData);
		if(iRet)
		{
			return 1;
		}
		iRet = createMatrixOfInteger32(Rhs + 1, _iRowsRet, _iColsRet, piData);
		break;
	case SCI_UINT32 :
		iRet = getMatrixOfUnsignedInteger32(_piAddress, &iRows, &iCols, &puiData);
		if(iRet)
		{
			return 1;
		}
		iRet = createMatrixOfUnsignedInteger32(Rhs + 1, _iRowsRet, _iColsRet, puiData);
		break;
	default :
		return 1;
		break;
	}

	return 0;
}

int	matrix_bool(int* _piAddress, int _iRowsRet, int _iColsRet)
{
	int iRet					= 0;
	int iIndex				= 0;
	int iRows					= 0;
	int iCols					= 0;

	int* piBool				= NULL;

	iRet = getMatrixOfBoolean(_piAddress, &iRows, &iCols, &piBool);
	if(iRet)
	{
		return 1;
	}

	iRet = createMatrixOfBoolean(Rhs + 1, _iRowsRet, _iColsRet, piBool);
	if(iRet)
	{
		return 1;
	}
	return 0;
}

/*--------------------------------------------------------------------------*/
