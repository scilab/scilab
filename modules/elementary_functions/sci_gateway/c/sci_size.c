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

//#define DEBUG_FILE
#define _NEW_TONIO_
/*--------------------------------------------------------------------------*/
int size_matrix(void);
int size_list(void);
void AddToFile(char *_pszFile, char * _pszVar, int _iVal);

char **g_pfname;
#define LOG_FILE "c:\\test.txt"
int toto = 0;
extern int C2F(intsize) _PARAMS((int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_size) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_

	g_pfname		= &fname;
	AddToFile(LOG_FILE, "START", toto);
	AddToFile(LOG_FILE, "RHS", Rhs);
	AddToFile(LOG_FILE, "LHS", Lhs);
	CheckRhs(1,2);
	CheckLhs(1,2);

	AddToFile(LOG_FILE, "TYPE", GetType(1));
	switch(GetType(1))
	{
	case sci_list  :
	case sci_tlist :
		size_list();
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
		size_matrix();
		break;
	default:
		OverLoad(1);
	}

#else
	C2F(intsize)(id);
#endif
	AddToFile(LOG_FILE, "\t\tEND", toto++);
	return 0;
}

int size_list(void)
{
	int iIndex			= 0;
	int iItemNumber		= 0;
	int *piItemType		= NULL;
	int iItemCount		= 0;
	int iRows			= 0;
	int iCols			= 0;

	double *pReturnData	= NULL;

	iGetListItemType(1, &iItemNumber, NULL);
	piItemType			= (int*)malloc(sizeof(int) * iItemNumber);
	iGetListItemType(1, &iItemNumber, piItemType);

	if(GetType(1) == sci_tlist && piItemType[0] == sci_strings)
	{
		char *pszData	= NULL;
		int *pLen		= NULL;
		int iLen		= 0;

		iGetListItemString(1, 0, &iRows, &iCols, NULL, NULL);
		pLen			= (int*)malloc(sizeof(int) * iRows * iCols);
		iGetListItemString(1, 0, &iRows, &iCols, pLen, NULL);
		iLen = iArraySum(pLen, 0, iRows * iCols);
		pszData			= (char*)malloc(sizeof(char) * iLen);
		iGetListItemString(1, 0, &iRows, &iCols, pLen, pszData);


		if(memcmp(pszData, "lss", strlen("lss")) == 0 || memcmp(pszData, "r", strlen("r")))
		{
			char *fname = *g_pfname;
			OverLoad(1);
			return 0;
		}
	}
	iAllocMatrixOfDouble(Rhs + 1, 1, 1, &pReturnData);
	pReturnData[0] = iItemNumber;
	LhsVar(1) = Rhs + 1;
	PutLhsVar();


/*
	//Just for test data retrieving with new list functions
	for(iIndex = 0 ; iIndex < iItemNumber ; iIndex++)
	{
		int iRows				= 0;
		int iCols				= 0;
		int *piVarName			= NULL;
		int *piPow				= NULL;
		
		double *pReturnRealData	= NULL;
		double *pReturnImgData	= NULL;

		switch(piItemType[iIndex])
		{
		case sci_matrix :
			iGetListItemDouble(1, iIndex, &iRows, &iCols, &pReturnRealData, &pReturnImgData);
			break;
		case sci_poly :
			iGetListItemPoly(1, iIndex, &piVarName, &iRows, &iCols, NULL, NULL, NULL);
			piPow = (int*)malloc(sizeof(int) * iRows * iCols);
			iGetListItemPoly(1, iIndex, &piVarName, &iRows, &iCols, piPow, &pReturnRealData, &pReturnImgData);
			break;
		case sci_boolean :
			break;
		case sci_sparse :
			break;
		case sci_boolean_sparse :
			break;
		case sci_ints :
			break;
		case sci_handles :
			break;
		case sci_strings :
			break;
		case sci_u_function :
			break;
		case sci_c_function :
			break;
		case sci_lib :
			break;
		case sci_list :
			break;
		case sci_tlist :
			break;
		case sci_mlist :
			break;
		case sci_lufact_pointer :
			break;
		default :
			break;
		}
		GetVarDimension(1, &iRows, &iCols);
	}
*/	return 0;
}

int size_matrix(void)
{
	int iRows					= 0;
	int iCols					= 0;
	int iRealData				= 0;
	int iImgData				= 0;
	int iIndex					= 0;
	int iOrient					= -1;
	
	double *pReturnRealData1	= NULL;
	double *pReturnRealData2	= NULL;


	if(Lhs > 2)
	{
		Error(42);
		return 0;
	}

	if(Rhs == 2)
	{
		if(Lhs != 1)
		{
			Error(41);
			return 0;
		}
		iOrient = iGetOrient(2);
	}

	GetVarDimension(1, &iRows, &iCols);
	CheckVarUsed(1);

	if(Lhs == 1) //
	{
		switch(iOrient)
		{
		case BY_ALL :
			iAllocMatrixOfDouble(Rhs + 1, 1, 1, &pReturnRealData1);
			pReturnRealData1[0] = iRows * iCols;
			break;
		case BY_MTLB:
			iAllocMatrixOfDouble(Rhs + 1, 1, 2, &pReturnRealData1);
			pReturnRealData1[0] = iRows;
			pReturnRealData1[1] = iCols;
			break;
		case BY_ROWS :
			iAllocMatrixOfDouble(Rhs + 1, 1, 1, &pReturnRealData1);
			pReturnRealData1[0] = iRows;
			break;
		case BY_COLS :
			iAllocMatrixOfDouble(Rhs + 1, 1, 1, &pReturnRealData1);
			pReturnRealData1[0] = iCols;
			break;
		}
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
	}
	else
	{
		iAllocMatrixOfDouble(Rhs + 1, 1, 1, &pReturnRealData1);
		iAllocMatrixOfDouble(Rhs + 2, 1, 1, &pReturnRealData2);
		pReturnRealData1[0] = iRows;
		pReturnRealData2[0] = iCols;
		LhsVar(1) = Rhs + 1;
		LhsVar(2) = Rhs + 2;
		PutLhsVar();
	}
	return 0;
}
/*--------------------------------------------------------------------------*/

void AddToFile(char *_pszFile, char * _pszVar, int _iVal)
{
#ifdef DEBUG_FILE
	FILE *f;
	char sz[1024];
	f = fopen(_pszFile, "a");
	sprintf(sz, "%s : %i\x0d\x0a", _pszVar, _iVal);
	fwrite(sz, sizeof(char), strlen(sz), f);
	fclose(f);
#endif
}