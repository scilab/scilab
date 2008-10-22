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


#define _NEW_TONIO_
/*--------------------------------------------------------------------------*/
void compare_list(int _iIsMaxi);
void compare_sparse(int _iIsMaxi);
int compare_double(int _iIsMaxi);
int compare_double_inside(int _iIsMaxi, int _iMode);

extern int C2F(intmaxi) _PARAMS((char *fname,int *id));
/*--------------------------------------------------------------------------*/
int C2F(sci_maxi) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iType			= 0;
	int iMaxi			= 1;
	int iMode			= 0;
	int iIndex			= 0;
	int iRet			= 0;
	CheckRhs(1,10000);
	CheckLhs(1,2);

	if(Fin == 17) // mini
		iMaxi = 0;

/*
Les differents appels : 
 - Matrix, chaine
 - Matrix, Matrix, ...
 - Matrix
 - List
*/

	iType = GetType(Rhs);
	switch(iType)
	{
	case sci_strings :
		if(Rhs == 2)
		{
			iMode = iGetOrient(2);
			if(GetType(1) != sci_matrix)
			{
				OverLoad(1);
				return 0;
			}
		}
		else
		{
			OverLoad(1);
			return 0;
		}
		compare_double_inside(iMaxi, iMode);
		break;
	case sci_matrix:
		for(iIndex = 0 ; iIndex < Rhs ; iIndex++)
		{
			if(GetType(iIndex + 1) != sci_matrix)
			{
				OverLoad(1);
				return 0;
			}
		}

		iRet = compare_double(iMaxi);
		if(iRet != 0)
			return 0;
		break;
	case sci_list :
		compare_list(iMaxi);
		break;
	case sci_sparse:
		C2F(ref2val)();
		Fin -= 6; //Ugly !!!
		if(iMaxi == 1)
			C2F(sci_spmax)(fname, fname_len);
		else
			C2F(sci_spmin)(fname, fname_len);
		break;
	default:
		OverLoad(1);
		break;
	}
#else
	C2F(intmaxi)(fname,id);
#endif
	return 0;
}

int compare_double(int _iIsMaxi)
{
	int iType		= 0;
	int iRows		= 0;
	int iCols		= 0;
	int	iRhsIndex	= 0;
	int iRefRows	= 0;
	int iRefCols	= 0;
	int iRefData	= 0;
	int iIndex		= 0;

	double *pReturnData1	= NULL;
	double *pReturnData2	= NULL;

	if(Rhs > 1)
	{
		for(iRhsIndex = 0 ; iRhsIndex < Rhs ; iRhsIndex++)
		{
			int iVar		= iRhsIndex + 1;
			int iRhsRows	= 0;
			int iRhsCols	= 0;
			int iRealData	= 0;


			GetVarDimension(iVar, &iRhsRows, &iRhsCols);
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

		iAllocMatrixOfDouble(Rhs + 1, iRows, iCols, &pReturnData1);
	
		if(Lhs == 2)
		{
			iAllocMatrixOfDouble(Rhs + 2, iRows, iCols, &pReturnData2);
			vDset(iRows * iCols, 1, pReturnData2, 1);
		}

		//Value
		//pReturnData1 = (double*)malloc(iRows * iCols * sizeof(double));

		//Index
		//pReturnData2 = (double*)malloc(iRows * iCols * sizeof(double));

		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRefRows, &iRefCols, &iRefData);
		if(iRefRows * iRefCols == 1)
			vDset(iRows * iCols, *stk(iRefData), pReturnData1, 1);
		else
			memcpy(pReturnData1, stk(iRefData), iRows * iCols * sizeof(double));

		for(iIndex = 1 ; iIndex < Rhs ; iIndex++)
		{
			int iVar			= iIndex + 1;
			int iCurRows		= 0;
			int iCurCols		= 0;
			int iCurData		= 0;
			double *pCurData	= NULL;
			int iInc			= 0;
			int iIndex2			= 0;
			int iIndex3			= 0;

			GetRhsVar(iVar, MATRIX_OF_DOUBLE_DATATYPE, &iCurRows, &iCurCols, &iCurData);
			pCurData = stk(iCurData);

			if(iCurRows == 1 && iCurCols == 1)
				iInc = 0;
			else
				iInc = 1;

			if(_iIsMaxi == 0)//mini
			{
				for(iIndex2 = 0 ; iIndex2 < iRows * iCols; iIndex2++)
				{
					if(pCurData[iIndex3] < pReturnData1[iIndex2] || ISNAN(pCurData[iIndex3]) == 1)
					{
						pReturnData1[iIndex2] = pCurData[iIndex3];
						if(Lhs == 2)
							pReturnData2[iIndex2] = (double)iVar;
					}
					iIndex3 += iInc;
				}
			}
			else//maxi
			{
				for(iIndex2 = 0 ; iIndex2 < iRows * iCols; iIndex2++)
				{
					if(pCurData[iIndex3] > pReturnData1[iIndex2] || ISNAN(pCurData[iIndex3]) == 1)
					{
						pReturnData1[iIndex2] = pCurData[iIndex3];
						if(Lhs == 2)
							pReturnData2[iIndex2] = (double)iVar;
					}
					iIndex3 += iInc;
				}
			}
		}//for

		//CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnData1);
		LhsVar(1) = Rhs + 1;

		if(Lhs == 2)
		{
			//CreateVarFromPtr(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnData2);
			LhsVar(2) = Rhs + 2;
		}

		PutLhsVar();
		//free(pReturnData1);
		//free(pReturnData2);
	}
	else // Rhs == 1
	{
		compare_double_inside(_iIsMaxi, 0);
	}
	return 0;
}

int compare_double_inside(int _iIsMaxi, int _iMode)
{
	int iRows		= 0;
	int iCols		= 0;
	int iRealData	= 0;
	int iIndex		= 0;
	int iUn			= 1;
	int iTemp		= 0;

	double *pdblRealData	= 0;
	double *pReturnData1	= NULL;
	double *pReturnData2	= NULL;

	GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
	pdblRealData		= stk(iRealData);

	if(iRows * iCols <= 0)
	{
		iRows = 0;
		iCols = 0;
		iAllocMatrixOfDouble(Rhs + 1, 0, 0, &pReturnData1);
		//CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnData1);
		LhsVar(1) = Rhs + 1;
		if(Lhs == 2)
		{
			iAllocMatrixOfDouble(Rhs + 2, 0, 0, &pReturnData2);
			//CreateVarFromPtr(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnData2);
			LhsVar(2) = Rhs + 2;
		}
		PutLhsVar();
		return 0;
	}

	switch(_iMode)
	{
	case BY_ALL :
		iAllocMatrixOfDouble(Rhs + 1, 1, 1, &pReturnData1);
		//pReturnData1 = (double*)malloc(sizeof(double));
		if(_iIsMaxi == 0)//mini
		{
			int iSize = iRows * iCols;
			iTemp = C2F(idmin)(&iSize, pdblRealData, &iUn);
		}
		else //maxi
		{
			int iSize = iRows * iCols;
			iTemp = C2F(idmax)(&iSize, pdblRealData, &iUn);
		}
		pReturnData1[0] = pdblRealData[iTemp-1];

		//CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iUn, &iUn, &pReturnData1);
		LhsVar(1) = Rhs + 1;
		//free(pReturnData1);

		if(Lhs == 2)
		{		
			if(iRows == 1 || iCols == 1)
			{
				iAllocMatrixOfDouble(Rhs + 2, 1, 1, &pReturnData2);
				//pReturnData2 = (double*)malloc(sizeof(double));
				pReturnData2[0] = iTemp;
				iCols = 1;
			}
			else
			{
				iCols = 2;
				iAllocMatrixOfDouble(Rhs + 2, 1, iCols, &pReturnData2);
				//pReturnData2 = (double*)malloc(iCols * sizeof(double));
				pReturnData2[0] = ((iTemp - 1) % iRows) + 1;
				pReturnData2[1] = ((iTemp - 1) / iRows) + 1;
			}
			//CreateVarFromPtr(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &iUn, &iCols, &pReturnData2);
			LhsVar(2) = Rhs + 2;
			//free(pReturnData2);
		}
		PutLhsVar();
		break;
	case BY_ROWS :
		iAllocMatrixOfDouble(Rhs + 1, 1, iCols, &pReturnData1);
		if(Lhs == 2)
			iAllocMatrixOfDouble(Rhs + 2, 1, iCols, &pReturnData2);

		//pReturnData1 = (double*)malloc(iCols * sizeof(double));
		//pReturnData2 = (double*)malloc(iCols * sizeof(double));
	
		if(_iIsMaxi == 0) //mini
		{
			for(iIndex = 0 ; iIndex < iCols ; iIndex++)
			{
				iTemp = C2F(idmin)(&iRows, &pdblRealData[iIndex*iRows], &iUn);
				pReturnData1[iIndex] = pdblRealData[iIndex*iRows + iTemp -1];
				if(Lhs == 2)
					pReturnData2[iIndex] = iTemp;
			}
		}
		else //maxi
		{
			for(iIndex = 0 ; iIndex < iCols ; iIndex++)
			{
				iTemp = C2F(idmax)(&iRows, &pdblRealData[iIndex*iRows], &iUn);
				pReturnData1[iIndex] = pdblRealData[iIndex*iRows + iTemp -1];
				if(Lhs == 2)
					pReturnData2[iIndex] = iTemp;
			}
		}

		//CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iUn, &iCols, &pReturnData1);
		LhsVar(1) = Rhs + 1;

		if(Lhs == 2)
		{
			//CreateVarFromPtr(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &iUn, &iCols, &pReturnData2);
			LhsVar(2) = Rhs + 2;
		}

		PutLhsVar();
		//free(pReturnData1);
		//free(pReturnData2);
		break;
	case BY_COLS :
		iAllocMatrixOfDouble(Rhs + 1, iRows, 1, &pReturnData1);
		if(Lhs == 2)
			iAllocMatrixOfDouble(Rhs + 2, iRows, 1, &pReturnData2);

		//pReturnData1 = (double*)malloc(iRows * sizeof(double));
		//pReturnData2 = (double*)malloc(iRows * sizeof(double));
	
		if(_iIsMaxi == 0) //mini
		{
			for(iIndex = 0 ; iIndex < iRows ; iIndex++)
			{
				iTemp = C2F(idmin)(&iCols, &pdblRealData[iIndex], &iRows);
				pReturnData1[iIndex] = pdblRealData[iIndex + (iTemp -1) * iRows];
				if(Lhs == 2)
					pReturnData2[iIndex] = iTemp;
			}
		}
		else //maxi
		{
			for(iIndex = 0 ; iIndex < iRows ; iIndex++)
			{
				iTemp = C2F(idmax)(&iCols, &pdblRealData[iIndex], &iRows);
				pReturnData1[iIndex] = pdblRealData[iIndex + (iTemp -1) * iRows];
				if(Lhs == 2)
					pReturnData2[iIndex] = iTemp;
			}
		}

		//CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iUn, &pReturnData1);
		LhsVar(1) = Rhs + 1;

		if(Lhs == 2)
		{
			//CreateVarFromPtr(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iUn, &pReturnData2);
			LhsVar(2) = Rhs + 2;
		}

		PutLhsVar();

		iRows = 0;
		iCols = 0;

		//free(pReturnData1);
		//free(pReturnData2);
		break;
	default: 
		break;
	}
	return 0;
}

void compare_list(int iIsMaxi)
{
}
/*--------------------------------------------------------------------------*/
