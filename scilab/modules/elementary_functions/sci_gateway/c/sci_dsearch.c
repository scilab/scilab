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

static int getMode(int _iPos, char *_pcMode);

/*--------------------------------------------------------------------------*/
int C2F(sci_dsearch) (char *fname,unsigned long fname_len)
{
	int i;
	int iRet								= 0;

	int* piAddr1						= NULL;
	int iRows1							= 0;
	int iCols1							= 0;
	double *pdblReal1				= NULL;

	int* piAddr2						= NULL;
	int iRows2							= 0;
	int iCols2							= 0;
	double *pdblReal2				= NULL;

	char cMode							= 0;

	int iRowsOcc						= 0;
	int iColsOcc						= 0;
	double *pdblRealOcc			= NULL;

	double *pdblRealInfo		= NULL;
	double *pdblRealInd			= NULL;

	CheckRhs(2,3);
	CheckLhs(1,3);

	iRet = getVarAddressFromPosition(1, &piAddr1);
	if(iRet)
	{
		return 1;
	}

	iRet = getVarAddressFromPosition(2, &piAddr2);
	if(iRet)
	{
		return 1;
	}

	if(getVarType(piAddr1) != sci_matrix || getVarType(piAddr2) != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	//get ch
	if(Rhs == 3)
	{
		iRet = getMode(3, &cMode);
		if(iRet)
		{
			return 1;
		}
	}
	else
		cMode = 'c';

	if(cMode != 'c' && cMode != 'd')
	{
		sciprint(_("%s: unknown char specifier (must be ''c'' or ''d'').\n"), fname);
		Error(999);
		return 0;
	}

	if(isVarComplex(piAddr1) || isVarComplex(piAddr2))
	{
		Error(202);
		return 0;
	}
	else
	{
		iRet = getMatrixOfDouble(piAddr2, &iRows2, &iCols2, &pdblReal2);
		if(iRet)
		{
			return 1;
		}

		//verif that val is in strict increasing order
		for(i = 1 ; i < iRows2 * iCols2 ; i++)
		{
			if(pdblReal2[i] < pdblReal2[i - 1])
			{
				sciprint(_("%s: the array val (arg 2) is not well ordered.\n"), fname);
				Error(999);
				return 0;
			}
		}
	}

	iRowsOcc = iRows2;
	iColsOcc = iCols2;
	if(cMode == 'd')
	{
		if( iRowsOcc * iColsOcc < 1)
		{
			sciprint(_("%s: Argument 2: argument 2 must not be an empty vector.\n"), fname);
			Error(999);
			return 0;
		}
	}
	else
	{// cMode == 'c'
		if( iRowsOcc * iColsOcc < 2)
		{
			sciprint(_("%s: in the interval case, argument 2 must be a vector with length > 1.\n"), fname);
			Error(999);
			return 0;
		}
		if(iRowsOcc == 1)
			iColsOcc--;
		else
			iRowsOcc--;
	}

	//Get X
	iRet = getMatrixOfDouble(piAddr1, &iRows1, &iCols1, &pdblReal1);
	if(iRet)
	{
		return 1;
	}

	if(Lhs >= 1)
	{
		iRet = allocMatrixOfDouble(Rhs + 1, iRows1, iCols1, &pdblRealInd);
	}

	if(Lhs >= 2)
	{
		iRet = allocMatrixOfDouble(Rhs + 2, iRowsOcc, iColsOcc, &pdblRealOcc);
	}

	if(Lhs >= 3)
	{
		iRet = allocMatrixOfDouble(Rhs + 3, 1, 1, &pdblRealInfo);
	}

	if(iRows1 == 0 || iCols1 == 0)
	{
		pdblRealInfo[0] = 0;
		if(Lhs >= 2)
		{
			memset(pdblRealOcc, 0x00, iRowsOcc * iColsOcc * sizeof(double));
		}
	}
	else
	{
		//go on for the computation
		if(cMode == 'c')
		{
			vDsearchC(pdblReal1, iRows1 * iCols1, pdblReal2, iRows2 * iCols2 - 1, pdblRealInd, pdblRealOcc, pdblRealInfo);
		}
		else
		{
			vDsearchD(pdblReal1, iRows1 * iCols1, pdblReal2, iRows2 * iCols2, pdblRealInd, pdblRealOcc, pdblRealInfo);
		}
	}

	if(Lhs >= 1)
	{
		LhsVar(1) = Rhs + 1;
	}
	if(Lhs >= 2)
	{
		LhsVar(2) = Rhs + 2;
	}
	if(Lhs >= 3)
	{
		LhsVar(3) = Rhs + 3;
	}

	PutLhsVar();
	return 0;
}

static int getMode(int _iPos, char *_pcMode)
{
	int iRet			= 0;
	int iRows			= 0;
	int iCols			= 0;
	int iMode			= 0;
	int* piAddr		= NULL;

	iRet = getVarAddressFromPosition(_iPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	if(getVarType(piAddr) == sci_strings)
	{
		int iLen						= 0;
		char pstMode[1][2]	= {""};

		iRet = getVarDimension(piAddr, &iRows, &iCols);
		if(iRet)
		{
			return 1;
		}

		if(iRows != 1 || iCols != 1)
		{
			Error(89);
			return 1;
		}

		iRet = getMatrixOfString(piAddr, &iRows, &iCols, &iLen, (char**)pstMode);
		if(iRet)
		{
			return 1;
		}

		*_pcMode = pstMode[0][0];
	}
	else
	{
		Error(44);
		return 2;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
