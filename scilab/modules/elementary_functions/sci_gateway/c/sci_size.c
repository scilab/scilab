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
#include "stdio.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "api_oldstack.h"

SciErr size_matrix(int* _piKey, int* _piAddress, int _iMode);
SciErr size_list(int* _piKey, int* _piAddress);

extern int C2F(intsize)(int* id);
/*--------------------------------------------------------------------------*/
int sci_size(char *fname, int* _piKey)
{
	SciErr sciErr;
	int iType 	= 0;
	int iMode		= 0;
	int* piAddr	= NULL;

	/*check inputs*/
	CheckRhs(1,2);
	CheckLhs(1,1000000);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(Rhs > 1)
	{
		int* piAddr2 = NULL;

		sciErr = getVarAddressFromPosition(_piKey, 2, &piAddr2);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = getProcessMode(_piKey, 2, piAddr, &iMode);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}


	sciErr = getVarType(_piKey, piAddr, &iType);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	switch(iType)
	{
	case sci_list  :
	case sci_tlist :
	case sci_mlist :
		if(Rhs != 1)
		{
			Error(39);
			return 0;
		}
		sciErr = size_list(_piKey, piAddr);
		break;
	case sci_matrix :
	case sci_poly :
	case sci_boolean :
	case sci_sparse :
	case sci_boolean_sparse :
	case sci_ints :
	case sci_strings :
		sciErr = size_matrix(_piKey, piAddr, iMode);
		break;
	}

	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	PutLhsVar();
	return 0;
/*
	static int id[6];
	C2F(intsize)(id);
	return 0;
*/

	}

SciErr size_list(int* _piKey, int* _piAddress)
{
	SciErr sciErr;
	//int iIndex			= 0;
	//int iItemNumber		= 0;
	//int *piItemType		= NULL;
	//int iItemCount		= 0;
	//int iRows			= 0;
	//int iCols			= 0;

	//double *pReturnData	= NULL;

	//iGetListItemType(1, NULL, &iItemNumber, NULL);
	//piItemType			= (int*)malloc(sizeof(int) * iItemNumber);
	//iGetListItemType(1, NULL, &iItemNumber, piItemType);

	//if(GetType(1) == sci_tlist && piItemType[0] == sci_strings)
	//{
	//	char *pszData	= NULL;
	//	int *pLen		= NULL;
	//	int iLen		= 0;

	//	iGetListItemString(1, 0, &iRows, &iCols, NULL, NULL);
	//	pLen			= (int*)malloc(sizeof(int) * iRows * iCols);
	//	iGetListItemString(1, 0, &iRows, &iCols, pLen, NULL);
	//	iLen = iArraySum(pLen, 0, iRows * iCols);
	//	pszData			= (char*)malloc(sizeof(char) * iLen);
	//	iGetListItemString(1, 0, &iRows, &iCols, pLen, pszData);


	//	if(memcmp(pszData, "lss", strlen("lss")) == 0 || memcmp(pszData, "r", strlen("r")))
	//	{
	//		char fname[] = "size";
	//		OverLoad(1);
	//		return 0;
	//	}
	//}
	//iAllocMatrixOfDouble(Rhs + 1, 1, 1, &pReturnData);
	//pReturnData[0] = iItemNumber;
	//LhsVar(1) = Rhs + 1;
	//PutLhsVar();
	return sciErr;
}

SciErr size_matrix(int* _piKey, int* _piAddress, int _iMode)
{
	SciErr sciErr;
	//int iRet		= 0;
	int iRows		= 0;
	int iCols		= 0;

	int iRowsOut	= 1;
	int iColsOut	= 0;

	double pdblReal[2] = {0,0};

	sciErr = getVarDimension(_piKey, _piAddress, &iRows, &iCols);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	if(Lhs == 1)
	{
		switch(_iMode)
		{
		case BY_MTLB : 
			iColsOut = 2;
			pdblReal[0] = iRows;
			pdblReal[1] = iCols;
			break;
		case BY_ROWS : 
			iColsOut = 1;
			pdblReal[0] = iRows;
			break;
		case BY_COLS : 
			iColsOut = 1;
			pdblReal[0] = iCols;
			break;
		case BY_ALL : 
			iColsOut = 1;
			pdblReal[0] = iRows * iCols;
			break;
		}
		
		sciErr = createMatrixOfDouble(_piKey, Rhs + 1, iRowsOut, iColsOut, pdblReal);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		LhsVar(1) = Rhs + 1;
	}
	else
	{
		int iRet = createScalarDoubleFromInteger(_piKey, Rhs + 1, &iRows);
		if(iRet)
		{
			SciErr sciErr;
			sciErr.iErr = iRet;
			return sciErr;
		}

		iRet = createScalarDoubleFromInteger(_piKey, Rhs + 2, &iCols);
		if(iRet)
		{
			SciErr sciErr;
			sciErr.iErr = iRet;
			return sciErr;
		}

		LhsVar(1) = Rhs + 1;
		LhsVar(2) = Rhs + 2;
	}

	//	if(Rhs == 1)
	//	{
	//		iRet = allocMatrixOfDouble(Rhs + 1, 1, 2, &pdblVal);
	//	}
	//	else
	//	{
	//		double* pdblVal		= NULL;
	//
	//		iRet = allocMatrixOfDouble(Rhs + 1, 1, 1, &pdblVal);
	//		if(iRet)
	//		{
	//			return 1;
	//		}

	//		switch(_iMode)
	//		{
	//			case BY_ROWS :
	//				pdblVal[0] = iCols;
	//				break;
	//			case BY_COLS :
	//				pdblVal[0] = iRows;
	//				break;
	//			default : //BY_ALL
	//				pdblVal[0] = iRows * iCols;
	//				break;
	//		}
	//	}
	//}
	return sciErr;
}
/*--------------------------------------------------------------------------*/
