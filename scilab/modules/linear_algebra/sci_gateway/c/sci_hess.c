/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "api_scilab.h"
#include <string.h> /* memcpy */
#include <stdio.h>
#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"
#include "stack3.h"
#include "MALLOC.h"
#include "hess.h"

static int getData(int* _piKey, int* adr, int* piRows, int* piCols, double** pPtrData);
static int  handleEmptyMatrix(int* _piKey);
static int  handleEyeMatrix(int* _piKey, int isComplex, double* pData);
static int zToRI(int* _piKey, int rows, int cols, doublecomplex* pData, doublecomplex* pH);
static double* allocH(int* _piKey, int isComplex, int rows, int cols);

static char* thisFunctionName=NULL;

int sci_hess(char *fname, int* _piKey)
{
	int iRows, iCols;
	int type;
	double* pData= NULL;

	int complexArg= 0;
	int ret= 0;

	int* adr1= NULL;
	/*   hess(A)  */
	thisFunctionName= fname;
	if (Rhs >= 1)
	{
		getVarAddressFromPosition(_piKey, 1, &adr1);

		getVarType(_piKey, adr1, &type);
		if(type != sci_matrix)
		{
			OverLoad(1);
			return 0;
		}
		else
		{
			double* pH= NULL;
			CheckRhs(1,1); /* H[,U]= hess(A) */
			CheckLhs(1,2);

			getData(_piKey, adr1, &iRows, &iCols, &pData);

			if( iRows != iCols)
			{
				Scierror(20,_("%s: Wrong size for input argument #%d: Square matrix expected.\n"),fname, 1);
			}
			else
			{
				complexArg= isVarComplex(_piKey, adr1);
				if( iCols == 0)
				{
					handleEmptyMatrix(_piKey);
				}
				else
				{
					if(iCols == -1 )
					{
						handleEyeMatrix(_piKey, complexArg, pData);
					}
					else
					{ /* neither empty nor eye matrix: at last the interesting case ! */
						double* pRes= NULL;
						pH= (Lhs==2) ? allocH(_piKey, complexArg, iRows, iCols) : NULL;

						ret = ret ? ret : iHessM(pData, iCols, complexArg, pH);

						allocMatrixOfDouble(_piKey, Rhs+1, iRows, iCols, &pRes);

						memcpy(pRes, pData, iRows * iCols * sizeof(double));

						if(complexArg)
						{
							zToRI(_piKey, iRows, iCols, (doublecomplex*)pData, (doublecomplex*)pH);
						}
					}
				}
			}
		}

		if(Lhs == 1)
		{
			LhsVar(1)= Rhs+1;
		}
		else
		{
			LhsVar(1)= Rhs+2;
			LhsVar(2)= Rhs+1;
		}
	}
	return ret;
}


/*===========================================================================*/
int getData(int* _piKey, int* adr, int* piRows, int* piCols, double** pPtrData)
{
	if(isVarComplex(_piKey, adr))
	{
		getComplexZMatrixOfDouble(_piKey, adr, piRows, piCols, ((doublecomplex**)pPtrData));
	}
	else
	{
		getMatrixOfDouble(_piKey, adr, piRows, piCols, pPtrData);
	}
	return 0;
}

int  handleEmptyMatrix(int* _piKey)
{
	int iRet 	= 0;
	int i 		= 0;
	
	for(i = 0 ; i < Lhs ; i++)
	{
		iRet = createEmptyMatrix(_piKey, Rhs + 1 + i);
		if(iRet)
		{
			return iRet;
		}
	}
}

int  handleEyeMatrix(int* _piKey, int isComplex, double* pData)
{
	if( Lhs >= 1 )
	{
		if(isComplex)
		{
			double* pReal;
			double* pImg;
			allocComplexMatrixOfDouble(_piKey, Rhs+1,-1, -1, &pReal, &pImg);
			*pReal= ((doublecomplex*)pData)->r;
			*pImg= ((doublecomplex*)pData)->i;
		}
		else
		{
			double* pRes;
			allocMatrixOfDouble(_piKey, Rhs+1, -1, -1, &pRes);
			*pRes= *pData;
		}
	}

	if(Lhs >= 2)
	{
		if(isComplex)
		{
			double* pHReal;
			double* pHImg;
			allocComplexMatrixOfDouble(_piKey, Rhs+2,-1, -1, &pHReal, &pHImg);
			*pHReal= 1.;
			*pHImg= 0.;
		}
		else
		{
			double* pH;
			allocMatrixOfDouble(_piKey, Rhs+2, -1, -1, &pH);
			*pH= 1.;
		}
	}
	return 0;
}

int zToRI(int* _piKey, int rows, int cols, doublecomplex* pData, doublecomplex* pH)
{
	double * pReal;
	double* pImg;
	allocComplexMatrixOfDouble(_piKey, Rhs+1, rows, cols, &pReal, &pImg);
	vGetPointerFromDoubleComplex(pData, rows * cols, pReal, pImg);
	FREE(pData);
	if(pH)
	{
		allocComplexMatrixOfDouble(_piKey, Rhs+2, rows, cols, &pReal, &pImg);
		vGetPointerFromDoubleComplex(pH, rows * cols, pReal, pImg);
		FREE(pH);
	}
	return 0;
}

double* allocH(int* _piKey, int isComplex, int rows, int cols)
{
	double* pH= NULL;
	if(isComplex)
	{
		if( (pH= (double*) MALLOC(rows * cols * sizeof(doublecomplex) ) )== NULL )
		{
			Scierror(999,_("%s: Cannot allocate more memory.\n"),thisFunctionName);
		}
	}
	else
	{
		allocMatrixOfDouble(_piKey, Rhs+2, rows, cols, &pH);
	}
	return pH;
}

