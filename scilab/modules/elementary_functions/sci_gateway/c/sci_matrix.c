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

SciErr matrix_double(int* _piAddress, int _iRowsRet, int _iColsRet);
SciErr matrix_bsparse(int* _piAddress, int _iRowsRet, int _iColsRet);
SciErr matrix_sparse(int* _piAddress, int _iRowsRet, int _iColsRet);
SciErr matrix_poly(int* _piAddress, int _iRowsRet, int _iColsRet);
SciErr matrix_string(int* _piAddress, int _iRowsRet, int _iColsRet);
SciErr matrix_int(int* _piAddress, int _iRowsRet, int _iColsRet);
SciErr matrix_bool(int* _piAddress, int _iRowsRet, int _iColsRet);

/*--------------------------------------------------------------------------*/
int sci_scimatrix(char *fname,unsigned long fname_len)
{
	SciErr sciErr;
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

	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(pvApiCtx, piAddr1, &iType);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

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
		sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		if(isVarComplex(pvApiCtx, piAddr2))
		{
			SciError(32);
			return 0;
		}

		sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows2, &iCols2, &pdblRealData2);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

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
		sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		if(isVarComplex(pvApiCtx, piAddr3) || isVarComplex(pvApiCtx, piAddr2))
		{
			SciError(32);
			return 0;
		}

		sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &iRows3, &iCols3, &pdblRealData3);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows2, &iCols2, &pdblRealData2);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}


		if(iRows3 * iCols3 != 1 || iRows2 * iCols2 != 1)
		{
			SciError(52);
			return 0;
		}
		else
		{
			iColsRet = (int)pdblRealData3[0];
			iRowsRet = (int)pdblRealData2[0];
			if(iColsRet < -1 || iRowsRet < -1)
			{
				SciError(116);
				return 0;
			}
		}
	}

	if(iColsRet == -1 && iRowsRet == -1)
	{
		SciError(42);
		return 0;
	}

	sciErr = getVarDimension(pvApiCtx, piAddr1, &iRows1, &iCols1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(iRowsRet == -1)
	{
		iRowsRet = (iRows1 * iCols1) / iColsRet;
	}

	if(iColsRet == -1)
	{
		iColsRet = (iRows1 * iCols1) / iRowsRet;
	}

	if(iRowsRet * iColsRet != iRows1 * iCols1)
	{
		SciError(60);
		return 0;
	}

	if(iRowsRet * iColsRet > MAX_INTERGER)
	{
		sciprint(_("%s: interger overflow.\n"), fname);
		SciError(9999);
	}

	switch(iType)
	{
	case sci_matrix :
		sciErr = matrix_double(piAddr1, iRowsRet, iColsRet);
		break;
	case sci_poly:
		sciErr = matrix_poly(piAddr1, iRowsRet, iColsRet);
		break;
	case sci_boolean :
		sciErr = matrix_bool(piAddr1, iRowsRet, iColsRet);
		break;
	case sci_sparse :
		sciErr = matrix_sparse(piAddr1, iRowsRet, iColsRet);
		break;
	case sci_boolean_sparse :
		sciErr = matrix_bsparse(piAddr1, iRowsRet, iColsRet);
		break;
	case sci_ints:
		sciErr = matrix_int(piAddr1, iRowsRet, iColsRet);
		break;
	case sci_strings:
		sciErr = matrix_string(piAddr1, iRowsRet, iColsRet);
		break;
	default :
		return 1;
		break;
	}

	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();

	return 0;
}

SciErr matrix_sparse(int* _piAddress, int _iRowsRet, int _iColsRet)
{
	SciErr sciErr;
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

	if(isVarComplex(pvApiCtx, _piAddress))
	{
		double *pOrder[3];
		sciErr = getComplexSparseMatrix(pvApiCtx, _piAddress, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		pOrder[0] = (double*)malloc(iNbItem * sizeof(double));
		pOrder[1] = (double*)malloc(iNbItem * sizeof(double));
		pOrder[2] = (double*)malloc(iNbItem * sizeof(double));

		sciErr = allocComplexSparseMatrix(pvApiCtx, Rhs + 1, _iRowsRet, _iColsRet, iNbItem, &piNbItemRowRet, &piColPosRet, &pdblRealRet, &pdblImgRet);
		if(sciErr.iErr)
		{
			return sciErr;
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
		sciErr = getSparseMatrix(pvApiCtx, _piAddress, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		pOrder[0] = (double*)malloc(iNbItem * sizeof(double));
		pOrder[1] = (double*)malloc(iNbItem * sizeof(double));
		pOrder[2] = (double*)malloc(iNbItem * sizeof(double));

		sciErr = allocSparseMatrix(pvApiCtx, Rhs + 1, _iRowsRet, _iColsRet, iNbItem, &piNbItemRowRet, &piColPosRet, &pdblRealRet);
		if(sciErr.iErr)
		{
			return sciErr;
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
	return sciErr;
}

SciErr matrix_bsparse(int* _piAddress, int _iRowsRet, int _iColsRet)
{
	SciErr sciErr;
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
	sciErr = getBooleanSparseMatrix(pvApiCtx, _piAddress, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	pOrder[0] = (int*)malloc(iNbItem * sizeof(int));
	pOrder[2] = (int*)malloc(iNbItem * sizeof(int));

	sciErr = allocBooleanSparseMatrix(pvApiCtx, Rhs + 1, _iRowsRet, _iColsRet, iNbItem, &piNbItemRowRet, &piColPosRet);
	if(sciErr.iErr)
	{
		return sciErr;
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
	return sciErr;
}

SciErr matrix_double(int* _piAddress, int _iRowsRet, int _iColsRet)
{
	SciErr sciErr;
	int iIndex				= 0;
	int iRows					= 0;
	int iCols					= 0;

	double* pdblReal	= NULL;
	double* pdblImg		= NULL;

	if(isVarComplex(pvApiCtx, _piAddress))
	{
		sciErr = getComplexMatrixOfDouble(pvApiCtx, _piAddress, &iRows, &iCols, &pdblReal, &pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = createComplexMatrixOfDouble(pvApiCtx, Rhs + 1, _iRowsRet, _iColsRet, pdblReal, pdblImg);
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

		sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, _iRowsRet, _iColsRet, pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}
	return sciErr;
}

SciErr matrix_poly(int* _piAddress, int _iRowsRet, int _iColsRet)
{
	SciErr sciErr;
	int i;
	int iRows					= 0;
	int iCols					= 0;
	int iMaxData			= 0;
	int iLen					= 0;
	int* piCoeff			= NULL;
	double **pdblReal	= NULL;
	double **pdblImg	= NULL;
	char pstVarName[16];

	sciErr = getPolyVariableName(pvApiCtx, _piAddress, pstVarName, &iLen);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	if(isVarComplex(pvApiCtx, _piAddress))
	{
		sciErr = getComplexMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, NULL, NULL, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		piCoeff	= (int*)malloc(sizeof(int) * iRows * iCols);
		sciErr	= getComplexMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, NULL, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		pdblReal	= malloc(sizeof(double*) * iRows * iCols);
		pdblImg		= malloc(sizeof(double*) * iRows * iCols);
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblReal[i] = malloc(sizeof(double) * piCoeff[i]);
			pdblImg[i]	= malloc(sizeof(double) * piCoeff[i]);
		}

		sciErr = getComplexMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = createComplexMatrixOfPoly(pvApiCtx, Rhs + 1, pstVarName, _iRowsRet, _iColsRet, piCoeff, pdblReal, pdblImg);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}
	else
	{
		sciErr = getMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, NULL, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		piCoeff	= (int*)malloc(sizeof(int) * iRows * iCols);
		sciErr = getMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, NULL);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		pdblReal	= malloc(sizeof(double*) * iRows * iCols);
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			pdblReal[i] = malloc(sizeof(double) * piCoeff[i]);
		}

		sciErr = getMatrixOfPoly(pvApiCtx, _piAddress, &iRows, &iCols, piCoeff, pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = createMatrixOfPoly(pvApiCtx, Rhs + 1, pstVarName, _iRowsRet, _iColsRet, piCoeff, pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}
	}

	//delete
	free(piCoeff);

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		free(pdblReal[i]);
	}
	free(pdblReal);

	if(isVarComplex(pvApiCtx, _piAddress))
	{
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			free(pdblImg[i]);
		}
		free(pdblImg);
	}

	return sciErr;
}

SciErr matrix_string(int* _piAddress, int _iRowsRet, int _iColsRet)
{
	SciErr sciErr;
	int i;
	int iRows				= 0;
	int iCols				= 0;
	int *piLen			= NULL;
	char **pstData	= NULL;

	sciErr = getMatrixOfString(pvApiCtx, _piAddress, &iRows, &iCols, NULL, NULL);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	piLen = malloc(sizeof(int) * iRows * iCols);
	sciErr = getMatrixOfString(pvApiCtx, _piAddress, &iRows, &iCols, piLen, NULL);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+1 for null termination
	}

	sciErr = getMatrixOfString(pvApiCtx, _piAddress, &iRows, &iCols, piLen, pstData);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, _iRowsRet, _iColsRet, pstData);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	free(piLen);
	for(i = 0 ; i < iRows * iCols ; i++)
	{
		free(pstData[i]);
	}
	free(pstData);
	return sciErr;
}

SciErr matrix_int(int* _piAddress, int _iRowsRet, int _iColsRet)
{
	SciErr sciErr;
	int iPrec								= 0;
	int iRows								= 0;
	int iCols								= 0;

	char* pcData						= NULL;
	short* psData						= NULL;
	int* piData							= NULL;
	unsigned char* pucData	= NULL;
	unsigned short* pusData	= NULL;
	unsigned int* puiData		= NULL;

	sciErr = getMatrixOfIntegerPrecision(pvApiCtx, _piAddress, &iPrec);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	switch(iPrec)
	{
	case SCI_INT8 :
		sciErr = getMatrixOfInteger8(pvApiCtx, _piAddress, &iRows, &iCols, &pcData);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = createMatrixOfInteger8(pvApiCtx, Rhs + 1, _iRowsRet, _iColsRet, pcData);
		break;
	case SCI_UINT8 :
		sciErr = getMatrixOfUnsignedInteger8(pvApiCtx, _piAddress, &iRows, &iCols, &pucData);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = createMatrixOfUnsignedInteger8(pvApiCtx, Rhs + 1, _iRowsRet, _iColsRet, pucData);
		break;
	case SCI_INT16 :
		sciErr = getMatrixOfInteger16(pvApiCtx, _piAddress, &iRows, &iCols, &psData);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = createMatrixOfInteger16(pvApiCtx, Rhs + 1, _iRowsRet, _iColsRet, psData);
		break;
	case SCI_UINT16 :
		sciErr = getMatrixOfUnsignedInteger16(pvApiCtx, _piAddress, &iRows, &iCols, &pusData);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = createMatrixOfUnsignedInteger16(pvApiCtx, Rhs + 1, _iRowsRet, _iColsRet, pusData);
		break;
	case SCI_INT32 :
		sciErr = getMatrixOfInteger32(pvApiCtx, _piAddress, &iRows, &iCols, &piData);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = createMatrixOfInteger32(pvApiCtx, Rhs + 1, _iRowsRet, _iColsRet, piData);
		break;
	case SCI_UINT32 :
		sciErr = getMatrixOfUnsignedInteger32(pvApiCtx, _piAddress, &iRows, &iCols, &puiData);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = createMatrixOfUnsignedInteger32(pvApiCtx, Rhs + 1, _iRowsRet, _iColsRet, puiData);
		break;
	default :
		return sciErr;
		break;
	}

	if(sciErr.iErr)
	{
		return sciErr;
	}

	return sciErr;
}

SciErr matrix_bool(int* _piAddress, int _iRowsRet, int _iColsRet)
{
	SciErr sciErr;
	int iIndex				= 0;
	int iRows					= 0;
	int iCols					= 0;

	int* piBool				= NULL;

	sciErr = getMatrixOfBoolean(pvApiCtx, _piAddress, &iRows, &iCols, &piBool);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	sciErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, _iRowsRet, _iColsRet, piBool);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	return sciErr;
}

/*--------------------------------------------------------------------------*/
