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

static SciErr getMode(int _iPos, char *_pcMode);

/*--------------------------------------------------------------------------*/
int C2F(sci_dsearch) (char *fname,unsigned long fname_len)
{
	SciErr sciErr;
	int i;

	int* piAddr1						= NULL;
	int iRows1							= 0;
	int iCols1							= 0;
	int iType1							= 0;
	double *pdblReal1				= NULL;

	int* piAddr2						= NULL;
	int iRows2							= 0;
	int iCols2							= 0;
	int iType2							= 0;
	double *pdblReal2				= NULL;

	char cMode							= 0;

	int iRowsOcc						= 0;
	int iColsOcc						= 0;
	double *pdblRealOcc			= NULL;

	double *pdblRealInfo		= NULL;
	double *pdblRealInd			= NULL;

	CheckRhs(2,3);
	CheckLhs(1,3);

	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(pvApiCtx, piAddr1, &iType1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(pvApiCtx, piAddr2, &iType2);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(iType1 != sci_matrix || iType2 != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	//get ch
	if(Rhs == 3)
	{
		sciErr = getMode(3, &cMode);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}
	else
	{
		cMode = 'c';
	}

	if(cMode != 'c' && cMode != 'd')
	{
		sciprint(_("%s: unknown char specifier (must be ''c'' or ''d'').\n"), fname);
		Error(999);
		return 0;
	}

	if(isVarComplex(pvApiCtx, piAddr1) || isVarComplex(pvApiCtx, piAddr2))
	{
		Error(202);
		return 0;
	}
	else
	{
		sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows2, &iCols2, &pdblReal2);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
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
		{
			iColsOcc--;
		}
		else
		{
			iRowsOcc--;
		}
	}

	//Get X
	sciErr = getMatrixOfDouble(pvApiCtx, piAddr1, &iRows1, &iCols1, &pdblReal1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(Lhs >= 1)
	{
		sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, iRows1, iCols1, &pdblRealInd);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}

	if(Lhs >= 2)
	{
		sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 2, iRowsOcc, iColsOcc, &pdblRealOcc);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
	}

	if(Lhs >= 3)
	{
		sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 3, 1, 1, &pdblRealInfo);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
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

static SciErr getMode(int _iPos, char *_pcMode)
{
	SciErr sciErr;
	int iRows			= 0;
	int iCols			= 0;
	int iMode			= 0;
	int* piAddr		= NULL;

	int iLen						= 0;
	char pstMode[1][2]	= {""};

	sciErr = getVarAddressFromPosition(pvApiCtx, _iPos, &piAddr);
	if(sciErr.iErr)
	{
		return sciErr;
	}


	sciErr = getVarDimension(pvApiCtx, piAddr, &iRows, &iCols);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	if(iRows == 1 && iCols == 1)
	{
		sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, &iLen, (char**)pstMode);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		*_pcMode = pstMode[0][0];
	}
	else
	{
		Error(89);
	}
	return sciErr;
}
/*--------------------------------------------------------------------------*/
