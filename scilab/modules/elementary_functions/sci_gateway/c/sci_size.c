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

int size_matrix(int* _piAddress, int _iMode);
int size_list(int* _piAddress);

extern int C2F(intsize)(int* id);
/*--------------------------------------------------------------------------*/
int C2F(sci_size) (char *fname,unsigned long fname_len)
{
	int iRet		= 0;
	int iMode		= 0;
	int* piAddr	= NULL;

	static int id[6];
	C2F(intsize)(id);
	return 0;

	CheckRhs(1,2);
	CheckLhs(1,2);

	iRet = getVarAddressFromPosition(1, &piAddr);
	if(iRet)
	{
		return 1;
	}

	if(Rhs == 2)
	{
		if(Lhs == 2)
		{
			Error(41);
			return 0;
		}
		iRet = getProcessMode(2, piAddr, &iMode);
		if(iRet)
		{
			return 1;
		}
	}
	switch(getVarType(piAddr))
	{
	case sci_list  :
	case sci_tlist :
	case sci_mlist :
		if(Rhs != 1)
		{
			Error(39);
			return 0;
		}
		iRet = size_list(piAddr);
		break;
	case sci_matrix :
	case sci_poly :
	case sci_boolean :
	case sci_sparse :
	case sci_boolean_sparse :
	case sci_matlab_sparse :
	case sci_ints :
	case sci_handles :
	case sci_strings :
		iRet = size_matrix(piAddr, iMode);
		break;
	default:
		OverLoad(1);
	}

	PutLhsVar();
	return 0;
}

int size_list(int* _piAddress)
{
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
	return 0;
}

int size_matrix(int* _piAddress, int _iMode)
{
	//int iRet		= 0;
	//int iRows		= 0;
	//int iCols		= 0;

	//iRet = getVarDimension(_piAddress, &iRows, &iCols);
	//if(iRows)
	//{
	//	return 1;
	//}

	//if(Lhs == 2)
	//{
	//	double dblRows	= iRows;
	//	double dblCols	= iCols;
	//	iRet = createMatrixOfDouble(Rhs + 1, 1, 1, &dblRows);
	//	if(iRet)
	//	{
	//		return 1;
	//	}
	//	LhsVar(1) = Rhs + 1;

	//	iRet = createMatrixOfDouble(Rhs + 2, 1, 1, &dblCols);
	//	if(iRet)
	//	{
	//		return 1;
	//	}
	//	LhsVar(2) = Rhs + 2;
	//}
	//else
	//{//take care of _iMode

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
	return 0;
}
/*--------------------------------------------------------------------------*/
