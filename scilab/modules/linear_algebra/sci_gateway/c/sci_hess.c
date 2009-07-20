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
#include "api_double.h"
#include "api_common.h"
#include <string.h> /* memcpy */
#include <stdio.h>
#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"
#include "stack3.h"
#include "MALLOC.h"
#include "hess.h"

static int getData(int* adr, int* piRows, int* piCols, double** pPtrData);
static int  handleEmptyMatrix(int isComplex);
static int  handleEyeMatrix( int isComplex, double* pData);
static int zToRI(int rows, int cols, doublecomplex* pData, doublecomplex* pH);
static double* allocH(int isComplex, int rows, int cols);

static char* thisFunctionName=NULL;

int C2F(inthess)(char *fname,unsigned long fname_len)
{
  int iRows, iCols;
  double* pData= NULL;

  int complexArg= 0;
  int ret= 0;

  int* adr1= NULL;
  /*   hess(A)  */
  thisFunctionName= fname;
  if (Rhs >= 1)
    {
      getVarAddressFromPosition(1, &adr1);
      if(getVarType(adr1)!=sci_matrix)
	{
	  OverLoad(1);
	  return 0;
	}
      else
	{
	  double* pH= NULL;
	  CheckRhs(1,1); /* H[,U]= hess(A) */
	  CheckLhs(1,2);

	  getData(adr1, &iRows, &iCols, &pData);

	  if( iRows != iCols)
	    {
	      Scierror(20,_("%s: Wrong size for input argument #%d: Square matrix expected.\n"),fname, 1);
	    }
	  else
	    {
	      complexArg= isVarComplex(adr1);
	      if( iCols == 0)
		{
		  handleEmptyMatrix(complexArg);
		}
	      else
		{
		  if(iCols == -1 )
		    {
		      handleEyeMatrix( complexArg, pData);
		    }
		  else
		    { /* neither empty nor eye matrix: at last the interesting case ! */
		      double* pRes= NULL;
		      pH= (Lhs==2) ? allocH(complexArg, iRows, iCols) : NULL;

		      ret = ret ? ret : iHessM(pData, iCols, complexArg, pH);

		      allocMatrixOfDouble(Rhs+1, iRows, iCols, &pRes);

		      memcpy(pRes, pData, iRows * iCols * sizeof(double));

		      if(complexArg)
			{
			  zToRI(iRows, iCols, (doublecomplex*)pData, (doublecomplex*)pH);
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
int getData(int* adr, int* piRows, int* piCols, double** pPtrData)
{
  if(isVarComplex(adr))
    {
      getComplexZMatrixOfDouble(adr, piRows, piCols, ((doublecomplex**)pPtrData));
    }
  else
    {
      getMatrixOfDouble(adr, piRows, piCols, pPtrData);
    }
  return 0;
}

int  handleEmptyMatrix(int isComplex)
{
  int i;
  for (i=0; i!= Lhs; ++i)
    {
      double* unused;
      if(isComplex)
	{
	  allocComplexMatrixOfDouble(Rhs+i+1,0, 0, &unused, &unused);
	}
      else
	{
	  allocMatrixOfDouble(Rhs+i+1, 0, 0, &unused);
	}
    }
  return 0;
}

int  handleEyeMatrix( int isComplex, double* pData)
{
  if( Lhs >= 1 )
    {
      if(isComplex)
	{
	  double* pReal;
	  double* pImg;
	  allocComplexMatrixOfDouble(Rhs+1,-1, -1, &pReal, &pImg);
	  *pReal= ((doublecomplex*)pData)->r;
	  *pImg= ((doublecomplex*)pData)->i;
	}
      else
	{
	  double* pRes;
	  allocMatrixOfDouble(Rhs+1, -1, -1, &pRes);
	  *pRes= *pData;
	}
    }
  if(Lhs >= 2)
    {
      if(isComplex)
	{
	  double* pHReal;
	  double* pHImg;
	  allocComplexMatrixOfDouble(Rhs+2,-1, -1, &pHReal, &pHImg);
	  *pHReal= 1.;
	  *pHImg= 0.;
	}
      else
	{
	  double* pH;
	  allocMatrixOfDouble(Rhs+2, -1, -1, &pH);
	  *pH= 1.;
	}
    }
  return 0;
}

int zToRI(int rows, int cols, doublecomplex* pData, doublecomplex* pH)
{
  double * pReal;
  double* pImg;
  allocComplexMatrixOfDouble(Rhs+1, rows, cols, &pReal, &pImg);
  vGetPointerFromDoubleComplex(pData, rows * cols, pReal, pImg);
  FREE(pData);
  if(pH)
    {
      allocComplexMatrixOfDouble(Rhs+2, rows, cols, &pReal, &pImg);
      vGetPointerFromDoubleComplex(pH, rows * cols, pReal, pImg);
      FREE(pH);
    }
  return 0;
}

double* allocH(int isComplex, int rows, int cols)
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
      if( allocMatrixOfDouble(Rhs+2, rows, cols, &pH) )
	{
	  Scierror(999,_("%s: stack size exceeded (Use stacksize function to increase it).\n"), thisFunctionName);
	}
    }
  return pH;
}
