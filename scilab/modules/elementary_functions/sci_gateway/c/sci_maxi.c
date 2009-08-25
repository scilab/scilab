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
#include "idmin.h"
#include "idmax.h"
#include "../../../sparse/includes/gw_sparse.h"
#include "api_scilab.h"
#include "Scierror.h"


int compare_list(int* _piAddress, int _iIsMini);
int compare_sparse(int* _piAddress, int _iIsMini);
int compare_double(int _iIsMini);
int compare_double_inside(int* _piAddress, int _iIsMini, int _iMode);

/*--------------------------------------------------------------------------*/
int C2F(sci_maxi) (char *fname,unsigned long fname_len)
{
	int i;
	int iRet			= 0;
	int iMode			= 0;
	int iMini			= 0;

	int *piAddr1		= NULL;

	CheckLhs(1,2);

	if(Fin == 17)
	{
		iMini = 1;
	}

	iRet = getVarAddressFromPosition(1, &piAddr1);
	if(iRet)
	{
		return 1;
	}


	//manage "c", "r", "m"
	if(Rhs == 2)
	{
		int *piAddr2		= NULL;
		iRet = getVarAddressFromPosition(2, &piAddr2);
		if(iRet)
		{
			return 1;
		}

		if(getVarType(piAddr2) == sci_strings)
		{
			iRet = getProcessMode(2, piAddr1, &iMode);
			if(iRet)
			{
				return 1;
			}
		}
	}

	switch(getVarType(piAddr1))
	{
	case sci_matrix :
		if(Rhs == 1)
		{
			iRet = compare_double_inside(piAddr1, iMini, iMode);
		}
		else
		{
			for(i = 2 ; i <= Rhs ; i++)
			{
				int* piAddr		= NULL;
				iRet = getVarAddressFromPosition(i, &piAddr);
				if(iRet)
				{
					return 1;
				}

				if(getVarType(piAddr) != sci_matrix)
				{
					OverLoad(1);
					return 0;
				}
			}
			iRet = compare_double(iMini);
			if(iRet)
			{
				return 1;
			}
		}
		break;
	case sci_list :
		iRet = compare_list(piAddr1, iMini);
		if(iRet)
		{
			return 1;
		}
		break;
	case sci_sparse:
		C2F(ref2val)();
		Fin -= 6; //Ugly !!!
		if(iMini)
			C2F(sci_spmin)(fname, fname_len);
		else
			C2F(sci_spmax)(fname, fname_len);
		break;
	default:
		OverLoad(1);
		return 0;
		break;	
	}

	PutLhsVar();
	return 0;
}

int compare_double(int _iIsMini)
{
	int i,j;
	int iRet							= 0;
	int iRows							= 0;
	int iCols							= 0;
	int iRefRows					= 0;
	int iRefCols					= 0;

	int* piAddr						= NULL;

	double *pdblReal			= NULL;
	double *pdblRealRet1	= NULL;
	double *pdblRealRet2	= NULL;

	for(i = 1 ; i <= Rhs ; i++)
	{
		int iVar			= i;
		int iRhsRows	= 0;
		int iRhsCols	= 0;

		iRet = getVarAddressFromPosition(iVar, &piAddr);
		if(iRet)
		{
			return 1;
		}

		if(isVarComplex(piAddr))
		{
			Err = i;
			Error(202);
		}

		getVarDimension(piAddr, &iRhsRows, &iRhsCols);
		if(iRhsRows * iRhsCols == 0)
		{
			Err = iVar;
			Error(45);
			return 1;
		}

		if(iVar == 1)
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
						Err = iVar;
						Error(42);
						return 2;
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

	iRet = allocMatrixOfDouble(Rhs + 1, iRows, iCols, &pdblRealRet1);
	if(iRet)
	{
		return 1;
	}

	if(Lhs == 2)
	{
		iRet = allocMatrixOfDouble(Rhs + 2, iRows, iCols, &pdblRealRet2);
		vDset(iRows * iCols, 1, pdblRealRet2, 1);
	}

	iRet = getMatrixOfDouble(piAddr, &iRefRows, &iRefCols, &pdblReal);
	if(iRefRows * iRefCols == 1)
		vDset(iRows * iCols, pdblReal[0], pdblRealRet1, 1);
	else
		memcpy(pdblRealRet1, pdblReal, iRows * iCols * sizeof(double));

	for(i = 1 ; i < Rhs ; i++)
	{
		int iVar				= i + 1;
		int iCurRows		= 0;
		int iCurCols		= 0;
		int iCurData		= 0;
		double *pdblCur	= NULL;
		int iInc				= 0;
		int iIndex2			= 0;
		int iIndex3			= 0;

		iRet = getMatrixOfDouble(piAddr, &iCurRows, &iCurCols, &pdblCur);
		if(iRet)
		{
			return 1;
		}

		if(iCurRows == 1 && iCurCols == 1)
			iInc = 0;
		else
			iInc = 1;

		if(_iIsMini == 0)
		{
			int k = 0;
			for(j = 0 ; j < iRows * iCols; j++)
			{
				if(pdblCur[k] < pdblRealRet1[j] || ISNAN(pdblCur[k]) == 1)
				{
					pdblRealRet1[j] = pdblCur[k];
					if(Lhs == 2)
					{
						pdblRealRet2[j] = (double)iVar;
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
						pdblRealRet2[j] = (double)iVar;
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
	return 0;
}

int compare_double_inside(int* _piAddress, int _iIsMini, int _iMode)
{
	int i;
	int iRet							= 0;
	int iRows							= 0;
	int iCols							= 0;
	int iValIndex					= 0;
	int iOne							= 1;//for fortran calls

	double *pdblReal			= NULL;
	double *pdblRealRet1	= NULL;
	double *pdblRealRet2	= NULL;

	if(isVarComplex(_piAddress))
	{
		Err = 1;
		Error(202);
	}

	iRet = getMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblReal);
	if(iRet)
	{
		return 1;
	}

	if(iRows * iCols <= 0)
	{
		iRows = 0;
		iCols = 0;
		iRet = allocMatrixOfDouble(Rhs + 1, 0, 0, &pdblRealRet1);
		if(iRet)
		{
			return 1;
		}
		LhsVar(1) = Rhs + 1;

		if(Lhs == 2)
		{
			iRet = allocMatrixOfDouble(Rhs + 2, 0, 0, &pdblRealRet2);
			if(iRet)
			{
				return 1;
			}
			LhsVar(2) = Rhs + 2;
		}
		return 0;
	}

	switch(_iMode)
	{
	case BY_ALL :
		iRet = allocMatrixOfDouble(Rhs + 1, 1, 1, &pdblRealRet1);
		if(iRet)
		{
			return 1;
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
				iRet = allocMatrixOfDouble(Rhs + 2, 1, 1, &pdblRealRet2);
				if(iRet)
				{
					return 1;
				}
				pdblRealRet2[0] = iValIndex;
			}
			else
			{
				iRet = allocMatrixOfDouble(Rhs + 2, 1, 2, &pdblRealRet2);
				if(iRet)
				{
					return 1;
				}
				pdblRealRet2[0] = ((iValIndex - 1) % iRows) + 1;
				pdblRealRet2[1] = ((iValIndex - 1) / iRows) + 1;
			}
			LhsVar(2) = Rhs + 2;
		}
		break;
	case BY_ROWS :
		iRet = allocMatrixOfDouble(Rhs + 1, 1, iCols, &pdblRealRet1);
		if(iRet)
		{
			return 1;
		}
		if(Lhs == 2)
		{
			iRet = allocMatrixOfDouble(Rhs + 2, 1, iCols, &pdblRealRet2);
			if(iRet)
			{
				return 1;
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
		iRet = allocMatrixOfDouble(Rhs + 1, iRows, 1, &pdblRealRet1);
		if(iRet)
		{
			return 1;
		}

		if(Lhs == 2)
		{
			iRet = allocMatrixOfDouble(Rhs + 2, iRows, 1, &pdblRealRet2);
			if(iRet)
			{
				return 1;
			}
		}
	
		if(_iIsMini == 0)
		{
			for(i = 0 ; i < iRows ; i++)
			{
				iValIndex					= C2F(idmin)(&iCols, &pdblReal[i], &iRows);
				pdblRealRet1[i]		= pdblReal[i + (iValIndex - 1) * iRows];
				if(Lhs == 2)
					pdblRealRet2[i] = iValIndex;
			}
		}
		else //maxi
		{
			for(i = 0 ; i < iRows ; i++)
			{
				iValIndex					= C2F(idmax)(&iCols, &pdblReal[i], &iRows);
				pdblRealRet1[i]		= pdblReal[i + (iValIndex - 1) * iRows];
				if(Lhs == 2)
					pdblRealRet2[i] = iValIndex;
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
	return 0;
}

int compare_list(int* _piAddress, int _iIsMini)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
