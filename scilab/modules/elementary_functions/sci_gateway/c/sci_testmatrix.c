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
#include "Scierror.h"
#include "api_scilab.h"
#include "api_oldstack.h"

#define MAGI_LETTER		'm'
#define FRK_LETTER		'f'
#define HILB_LETTER		'h'

char getGenerateMode(int* _piAddress);

/*--------------------------------------------------------------------------*/
int sci_testmatrix(char *fname, int* _piKey)
{
	int iRet						= 0;

	int iRows1					= 0;
	int iCols1					= 0;
	int* piAddr1				= NULL;
	char cMode					= 0;


	int iRows2					= 0;
	int iCols2					= 0;
	int* piAddr2				= NULL;
	int iDim						= 0;

	double *pdblRealRet = NULL;
	
	CheckRhs(2,2);
	CheckLhs(1,1);

	/*check input 1*/
	iRet = getVarAddressFromPosition(1, &piAddr1, _piKey);
	if(iRet)
	{
		return 1;
	}

	cMode = getGenerateMode(piAddr1);

/*	GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &iRows, &iCols, &szRealData);
	cMode = (int)*szRealData[0];

	if(iIsComplex(2))
	{
		//voir comment on ajoute une string dans les messages d'erreur
		//"%dth argument must be a real scalar
		Error(51);
		return 0;
	}

	GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
	if(iRows != 1 || iCols != 1)
	{
		//voir comment on ajoute une string dans les messages d'erreur
		//"%dth argument must be a real scalar
		Error(51);
		return 0;
	}
	iVal = (int)dabss(stk(iRealData)[0]);

	iAllocMatrixOfDouble(Rhs + 1, iVal, iVal, &pReturnRealData);
	switch(cMode)
	{
	case FRK_LETTER :
		franck_matrix(iVal, pReturnRealData);
		break;
	case HILB_LETTER :
		hilb_matrix(iVal, pReturnRealData);
		break;
	default : //Magic case and others
		magic_matrix(iVal, pReturnRealData);
		break;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
*/	return 0;
}

char getGenerateMode(int* _piAddress)
{
	int iRet		= 0;
	int iRows		= 0;
	int iCols		= 0;
	int piLen[1];

	char* pstData[1];

	if(getVarType(_piAddress) != sci_strings)
	{
		return 0;
	}

	iRet = getVarDimension(_piAddress, &iRows, &iCols);
	if(iRet || iRows != 1 || iCols != 1)
	{
		return 0;
	}

	iRet = getMatrixOfString(_piAddress, &iRows, &iCols, piLen, NULL);
	if(iRet)
	{
		return 0;
	}

	pstData[0] = malloc(sizeof(char) * (piLen[0] + 1));//+1 for null termination
	iRet = getMatrixOfString(_piAddress, &iRows, &iCols, piLen, (char**)pstData);
	if(iRet)
	{
		return 0;
	}
	
	return pstData[0][0];
}
/*--------------------------------------------------------------------------*/
