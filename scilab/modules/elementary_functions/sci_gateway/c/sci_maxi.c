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
#include "MALLOC.h"
#include "basic_functions.h"
#include "idmin.h"
#include "idmax.h"
#include "../../../sparse/includes/gw_sparse.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "api_oldstack.h"

int func_comp(char* fname, int _iMini, int* _piKey);

SciErr compare_list(int* _piKey, char* _pstName, int* _piAddress, int _iIsMini);
SciErr compare_multiple_double(int* _piKey, char* _pstName, int _iIsMini);
SciErr compare_double_inside(int* _piKey, int* _piAddress, int _iIsMini, int _iMode);

static SciErr compare_double(int* _piKey, int _iIsMini, int** _piAddr, int _iNbItem);

/*--------------------------------------------------------------------------*/
int sci_maxi(char *fname, int* _piKey)
{
	return func_comp(fname, 0, _piKey);
}

int sci_mini(char *fname, int* _piKey)
{
	return func_comp(fname, 1, _piKey);
}


int func_comp(char* fname, int _iMini, int* _piKey)
{
	SciErr sciErr;
	int i;
	int iMode				= 0;
	int iType1			= 0;
	int iModeActive = 0;
	int *piAddr1		= NULL;

	CheckLhs(1,2);

	sciErr = getVarAddressFromPosition(_piKey, 1, &piAddr1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = getVarType(_piKey, piAddr1, &iType1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	//manage "c", "r", "m"
	if(Rhs == 2)
	{
		int *piAddr2	= NULL;
		int iType2		= 0;

		if(iType1 != sci_matrix) 
		{
			Scierror(999, _("%s: More than one argument can be use only with double matrix in first argument.\n"), fname);
			return 0;
		}

		sciErr = getVarAddressFromPosition(_piKey, 2, &piAddr2);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = getVarType(_piKey, piAddr2, &iType2);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		if(iType2 == sci_strings)
		{
			sciErr = getProcessMode(_piKey, 2, piAddr1, &iMode);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
			iModeActive = 1;
		}
	}

	switch(iType1)
	{
	case sci_matrix :
		if(Rhs == 1 || (Rhs == 2 && iModeActive))
		{
			sciErr = compare_double_inside(_piKey, piAddr1, _iMini, iMode);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
		}
		else
		{
			for(i = 2 ; i <= Rhs ; i++)
			{
				int* piAddr		= NULL;
				int iType = 0;
				sciErr = getVarAddressFromPosition(_piKey, i, &piAddr);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}

				sciErr = getVarType(_piKey, piAddr, &iType);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}

				if(iType != sci_matrix)
				{
					OverLoad(1);
					return 0;
				}
			}
			sciErr = compare_multiple_double(_piKey, fname, _iMini);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
		}
		break;
	case sci_list :
		sciErr = compare_list(_piKey, fname, piAddr1, _iMini);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		break;
	case sci_sparse:
		Fin -= 6; //Ugly !!!
		if(_iMini)
			C2F(sci_spmin)(fname, _piKey);
		else
			C2F(sci_spmax)(fname, _piKey);
		break;
	default:
		OverLoad(1);
		return 0;
		break;
	}

	PutLhsVar();
	return 0;
}

SciErr compare_multiple_double(int* _piKey, char* _pstName, int _iIsMini)
{
	SciErr sciErr;
	int i;
	char* fname			= _pstName;
	int iNbItem			= Rhs;
	int **piItem		= NULL;

	piItem = (int**)MALLOC(sizeof(int*) * iNbItem);
	for(i = 0 ; i < Rhs ; i++)
	{
		int iType = 0;
		sciErr = getVarAddressFromPosition(_piKey, i + 1, &piItem[i]);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = getVarType(_piKey, piItem[i], &iType);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		if(iType != sci_matrix)
		{
			OverLoad(1);
			return sciErr;
		}
	}
	
	return compare_double(_piKey, _iIsMini, piItem, iNbItem);
}

SciErr compare_double_inside(int* _piKey, int* _piAddress, int _iIsMini, int _iMode)
{
	SciErr sciErr;
	int i;
	int iRows							= 0;
	int iCols							= 0;
	int iValIndex					= 0;
	int iOne							= 1;//for fortran calls

	double *pdblReal			= NULL;
	double *pdblRealRet1	= NULL;
	double *pdblRealRet2	= NULL;

	sciErr.iErr						= 0;

	if(isVarComplex(_piKey, _piAddress))
	{
		Err = 1;
		SciError(202);
		return sciErr;
	}

	sciErr = getMatrixOfDouble(_piKey, _piAddress, &iRows, &iCols, &pdblReal);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	if(iRows * iCols <= 0)
	{
		iRows = 0;
		iCols = 0;
		sciErr = allocMatrixOfDouble(_piKey, Rhs + 1, 0, 0, &pdblRealRet1);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		LhsVar(1) = Rhs + 1;
		if(Lhs == 2)
		{
			sciErr = allocMatrixOfDouble(_piKey, Rhs + 2, 0, 0, &pdblRealRet2);
			if(sciErr.iErr)
			{
				return sciErr;
			}

			LhsVar(2) = Rhs + 2;
		}
		return sciErr;
	}

	switch(_iMode)
	{
	case BY_ALL :
		sciErr = allocMatrixOfDouble(_piKey, Rhs + 1, 1, 1, &pdblRealRet1);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		if(_iIsMini)
		{
			int iSize = iRows * iCols;
			iValIndex = C2F(idmin)(&iSize, pdblReal, &iOne);
		}
		else //maxi
		{
			int iSize = iRows * iCols;
			iValIndex = C2F(idmax)(&iSize, pdblReal, &iOne);
		}

		pdblRealRet1[0] = pdblReal[iValIndex-1];
		LhsVar(1) = Rhs + 1;

		if(Lhs == 2)
		{
			if(iRows == 1 || iCols == 1)
			{
				sciErr = allocMatrixOfDouble(_piKey, Rhs + 2, 1, 1, &pdblRealRet2);
				if(sciErr.iErr)
				{
					return sciErr;
				}

				pdblRealRet2[0] = iValIndex;
			}
			else
			{
				sciErr = allocMatrixOfDouble(_piKey, Rhs + 2, 1, 2, &pdblRealRet2);
				if(sciErr.iErr)
				{
					return sciErr;
				}

				pdblRealRet2[0] = ((iValIndex - 1) % iRows) + 1;
				pdblRealRet2[1] = ((iValIndex - 1) / iRows) + 1;
			}
			LhsVar(2) = Rhs + 2;
		}
		break;
	case BY_ROWS :
		sciErr = allocMatrixOfDouble(_piKey, Rhs + 1, 1, iCols, &pdblRealRet1);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		if(Lhs == 2)
		{
			sciErr = allocMatrixOfDouble(_piKey, Rhs + 2, 1, iCols, &pdblRealRet2);
			if(sciErr.iErr)
			{
				return sciErr;
			}
		}

		if(_iIsMini)
		{
			for(i = 0 ; i < iCols ; i++)
			{
				iValIndex					= C2F(idmin)(&iRows, &pdblReal[i * iRows], &iOne);
				pdblRealRet1[i]		= pdblReal[i * iRows + iValIndex - 1];
				if(Lhs == 2)
				{
					pdblRealRet2[i] = iValIndex;
				}
			}
		}
		else //maxi
		{
			for(i = 0 ; i < iCols ; i++)
			{
				iValIndex					= C2F(idmax)(&iRows, &pdblReal[i*iRows], &iOne);
				pdblRealRet1[i]		= pdblReal[i * iRows + iValIndex - 1];
				if(Lhs == 2)
				{
					pdblRealRet2[i]	= iValIndex;
				}
			}
		}

		LhsVar(1) = Rhs + 1;
		if(Lhs == 2)
		{
			LhsVar(2) = Rhs + 2;
		}
		break;
	case BY_COLS :
		sciErr = allocMatrixOfDouble(_piKey, Rhs + 1, iRows, 1, &pdblRealRet1);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		if(Lhs == 2)
		{
			sciErr = allocMatrixOfDouble(_piKey, Rhs + 2, iRows, 1, &pdblRealRet2);
			if(sciErr.iErr)
			{
				return sciErr;
			}
		}

		if(_iIsMini)
		{
			for(i = 0 ; i < iRows ; i++)
			{
				iValIndex					= C2F(idmin)(&iCols, &pdblReal[i], &iRows);
				pdblRealRet1[i]		= pdblReal[i + (iValIndex - 1) * iRows];
				if(Lhs == 2)
				{
					pdblRealRet2[i] = iValIndex;
				}
			}
		}
		else //maxi
		{
			for(i = 0 ; i < iRows ; i++)
			{
				iValIndex					= C2F(idmax)(&iCols, &pdblReal[i], &iRows);
				pdblRealRet1[i]		= pdblReal[i + (iValIndex - 1) * iRows];
				if(Lhs == 2)
				{
					pdblRealRet2[i] = iValIndex;
				}
			}
		}

		LhsVar(1) = Rhs + 1;
		if(Lhs == 2)
		{
			LhsVar(2) = Rhs + 2;
		}
		break;
	default:
		break;
	}
	return sciErr;
}

SciErr compare_list(int* _piKey, char* _pstName, int* _piAddress, int _iIsMini)
{
	SciErr sciErr;
	int i;
	char* fname		= _pstName;
	int iNbItem		= 0;
	int **piItem	= NULL;

	sciErr = getListItemNumber(_piKey, _piAddress, &iNbItem);
	if(sciErr.iErr)
	{
		return sciErr;
	}


	piItem = (int**)MALLOC(sizeof(int*) * iNbItem);
	for(i = 0 ; i < iNbItem ; i++)
	{
		int iType = 0;
		sciErr = getListItemAddress(_piKey, _piAddress, i+1, &piItem[i]);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		sciErr = getVarType(_piKey, piItem[i], &iType);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		if(iType != sci_matrix)
		{
			OverLoad(1);
			return sciErr;
		}
	}

	return compare_double(_piKey, _iIsMini, piItem, iNbItem);
}

static SciErr compare_double(int* _piKey, int _iIsMini, int** _piAddr, int _iNbItem)
{
	SciErr sciErr;
	int i,j;
	int iRows							= 0;
	int iCols							= 0;
	int iRefRows					= 0;
	int iRefCols					= 0;

	double *pdblReal			= NULL;
	double *pdblRealRet1	= NULL;
	double *pdblRealRet2	= NULL;

	for(i = 0 ; i < _iNbItem ; i++)
	{
		int iRhsRows	= 0;
		int iRhsCols	= 0;

		if(isVarComplex(_piKey, _piAddr[i]))
		{
			Err = i;
			SciError(202);
			return sciErr;
		}

		sciErr = getVarDimension(_piKey, _piAddr[i], &iRhsRows, &iRhsCols);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		if(iRhsRows * iRhsCols == 0)
		{
			Err = i + 1;
			SciError(45);
			return sciErr;
		}

		if(i == 0)
		{
			iRows = iRhsRows;
			iCols = iRhsCols;
		}
		else
		{
			if(iRhsRows != 1 || iRhsCols != 1)
			{
				if(iRhsRows != iRows || iRhsCols != iCols)
				{
					if(iRows * iCols != 1)
					{
						Err = i + 1;
						SciError(42);
						return sciErr;
					}
					else
					{
						iRows = iRhsRows;
						iCols = iRhsCols;
					}
				}
			}
		}
	}//for

	sciErr = allocMatrixOfDouble(_piKey, Rhs + 1, iRows, iCols, &pdblRealRet1);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	if(Lhs == 2)
	{
		sciErr = allocMatrixOfDouble(_piKey, Rhs + 2, iRows, iCols, &pdblRealRet2);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		vDset(iRows * iCols, 1, pdblRealRet2, 1);
	}

	sciErr = getMatrixOfDouble(_piKey, _piAddr[0], &iRefRows, &iRefCols, &pdblReal);
	if(sciErr.iErr)
	{
		return sciErr;
	}

	if(iRefRows * iRefCols == 1)
	{
		vDset(iRows * iCols, pdblReal[0], pdblRealRet1, 1);
	}
	else
	{
		memcpy(pdblRealRet1, pdblReal, iRows * iCols * sizeof(double));
	}

	for(i = 1 ; i < _iNbItem ; i++)
	{
		int iVar				= i + 1;
		int iCurRows		= 0;
		int iCurCols		= 0;
		int iCurData		= 0;
		double *pdblCur	= NULL;
		int iInc				= 0;
		int iIndex2			= 0;
		int iIndex3			= 0;

		sciErr = getMatrixOfDouble(_piKey, _piAddr[i], &iCurRows, &iCurCols, &pdblCur);
		if(sciErr.iErr)
		{
			return sciErr;
		}

		if(iCurRows == 1 && iCurCols == 1)
		{
			iInc = 0;
		}
		else
		{
			iInc = 1;
		}

		if(_iIsMini)
		{
			int k = 0;
			for(j = 0 ; j < iRows * iCols; j++)
			{
				if(pdblCur[k] < pdblRealRet1[j] || ISNAN(pdblCur[k]) == 1)
				{
					pdblRealRet1[j] = pdblCur[k];
					if(Lhs == 2)
					{
						pdblRealRet2[j] = (double)(i + 1);
					}
				}
				k += iInc;
			}
		}
		else//maxi
		{
			int k = 0;
			for(j = 0 ; j < iRows * iCols; j++)
			{
				if(pdblCur[k] > pdblRealRet1[j] || ISNAN(pdblCur[k]) == 1)
				{
					pdblRealRet1[j] = pdblCur[k];
					if(Lhs == 2)
					{
						pdblRealRet2[j] = (double)(i + 1);
					}
				}
				k += iInc;
			}
		}
	}//for

	LhsVar(1) = Rhs + 1;

	if(Lhs == 2)
	{
		LhsVar(2) = Rhs + 2;
	}
	return sciErr;
}

/*--------------------------------------------------------------------------*/
