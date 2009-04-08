
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2009 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "machine.h"
#include "doublecomplex.h"
#include "MALLOC.h"

#include "sas_schur.h"

/*
 * Lapack routines used to perform the schur decompositions
 */

extern void C2F(dgges)(char const jobVsL[1], char const jobVsR[1], char const sort[1],  int* (*select)(double*, double*, double*), int const* n
		       , double* a, int const* ldA, double* b, int const* ldB, int* sDim, double* alphaR, double* alphaI, double* beta
		       , double* VSL, int const* ldVSL, double* VSR, int const* ldVSR, double* work, int const* lWork, int* bWork, int* info );

extern void C2F(dgees)(char const jobVs[1], char const sort[1],  int* (*select)(double*, double*), int const* n, double* a, int const* ldA
		       , int * sDim, double* wR, double* wI, double* VS, int const* ldVS, double* work, int const* lWork, int* bWork, int* info );

extern void C2F(zgges)(char const jobVsL[1], char const jobVsR[1], char const sort[1],  int* (*select)(doublecomplex*, doublecomplex*)
		       , int const* n, doublecomplex* a, int const* ldA, doublecomplex* b, int const* ldB, int* sDim
		       , doublecomplex* alpha, doublecomplex* beta, doublecomplex* VSL, int const* ldVSL, doublecomplex* VSR, int const* ldVSR
		       , doublecomplex* work, int const* lWork, double* rWork, int* bWork, int* info);

extern void C2F(zgees)(char const jobVs[1], char const sort[1],  int* (*select)(doublecomplex*), int const* n, doublecomplex* a, int const* ldA
		       , int * sDim, doublecomplex* w, doublecomplex* VS, int const* ldVS, doublecomplex* work, int const* lWork, double* rWork
		       , int* bWork, int* info );

/*
 * functions allocating workspace memory for schur decompositions : first try to allocate optimal workspace by querying Lapack. If MALLOC for optimal
 * worspace fail, try to allocate minimal workspace. If that also fails, return NULL.
 * Set *allocated to nb of allocated elements (double[complex]not bytes!).
 *
 * in :
 *
 * iCols int in : nb of rows/cols of the matri[x|ces]
 * computeSchurVectors int (boolean semantics) in : whether schur vectors are to be computed.
 *
 * out :
 *
 * allocated int* out: nb of allocated elements (double[complex]not bytes!).
 *
 * @return ptr to allocated workspace, NULL if allocation failed.
 */

static double* iAllocDggesWorkspace(int iCols, int computeSchurVectors, int* allocated)
{
  int info;
  int query=-1;
  double optim;
  double* ret= NULL;
  C2F(dgges)(computeSchurVectors ? "V" : "N", computeSchurVectors ? "N" : "V", "N", NULL, &iCols, NULL, &iCols, NULL, &iCols
	     , NULL, NULL, NULL, NULL, NULL, &iCols, NULL, &iCols, &optim, &query, NULL, &info); 
  *allocated= (int) optim;
  ret= (double*) MALLOC( (*allocated) * sizeof(double));
  if(!ret)
    {
      *allocated= 8 * iCols + 16;
      ret= (double*) MALLOC( (*allocated) * sizeof(double));
      if(!ret)
	{
	  *allocated= 0;
	}
    }
  return ret;
}
static double* iAllocDgeesWorkspace(int iCols, int computeSchurVectors, int* allocated)
{
  int info;
  int query=-1;
  double optim;
  double* ret= NULL;
  C2F(dgees)(computeSchurVectors ? "V" : "N", "N", NULL, &iCols, NULL, &iCols, NULL, NULL, NULL, NULL, &iCols, &optim, &query, NULL, &info); 
  *allocated= (int) optim;
  ret= (double*) MALLOC( (*allocated) * sizeof(double));
  if(!ret)
    {
      *allocated= 3 * iCols;
      ret= (double*) MALLOC( (*allocated) * sizeof(double));
      if(!ret)
	{
	  *allocated= 0;
	}
    }
  return ret;
}
static doublecomplex* iAllocZggesWorkspace(int iCols, int computeSchurVectors, int* allocated)
{
  int info;
  int query=-1;
  doublecomplex optim;
  doublecomplex* ret= NULL;
  C2F(zgges)(computeSchurVectors ? "V" : "N", computeSchurVectors ? "V" : "N", "N", NULL, &iCols, NULL, &iCols, NULL, &iCols
	     , NULL, NULL, NULL, NULL, &iCols, NULL, &iCols, &optim, &query, NULL, NULL, &info);

  *allocated= (int) optim.r;
  ret= (doublecomplex*) MALLOC( (*allocated) * sizeof(doublecomplex));
  if(!ret)
    {
      *allocated= 2 * iCols;
      ret= (doublecomplex*) MALLOC( (*allocated) * sizeof(doublecomplex));
      if(!ret)
	{
	  *allocated= 0;
	}
    }
  return ret;
}
static doublecomplex* iAllocZgeesWorkspace(int iCols, int computeSchurVectors, int* allocated)
{
  int info;
  int query=-1;
  doublecomplex optim;
  doublecomplex* ret= NULL;
  C2F(zgees)(computeSchurVectors ? "V" : "N", "N", NULL, &iCols, NULL, &iCols, NULL, NULL, NULL, &iCols, &optim, &query, NULL, NULL, &info); 
  *allocated= (int) optim.r;
  ret= (doublecomplex*) MALLOC( (*allocated) * sizeof(doublecomplex));
  if(!ret)
    {
      *allocated= 2 * iCols;
      ret= (doublecomplex*) MALLOC( (*allocated) * sizeof(doublecomplex));
      if(!ret)
	{
	  *allocated= 0;
	}
    }
  return ret;
}

/*
 * part of the API. Cf. sas_schur.h
 */
int iSchurM(double* pData1, double* pData2, int iCols, int complexArgs, double* pLhsOpt1, double* pLhsOpt2)
{
  int info= 0;
  int* pBwork= NULL;
  int worksize=0;
  int sDim= 0;
  double* pRwork= NULL; // 'd' iCols
  doublecomplex* pCplxWork= NULL;

  pBwork= (int*) MALLOC( ( pData2 ? 2 : 1)*iCols * sizeof(int));
  switch ( (complexArgs ? 1: 0) + (pData2 ? 2 : 0) )
    {
    case 1+2 : /* zgges */
      {
	doublecomplex* pAlpha= NULL;
	doublecomplex* pBeta= NULL;
	info= ( (pCplxWork= iAllocZggesWorkspace(iCols, pLhsOpt1 ? 1 :0, &worksize))
		&&(pRwork= (double*) MALLOC(8 * iCols * sizeof(double)))
		&&(pAlpha= (doublecomplex*)MALLOC( iCols * sizeof(doublecomplex)))
		&&(pBeta= (doublecomplex*)MALLOC( iCols * sizeof(doublecomplex))))
	  ? ( C2F(zgges)(pLhsOpt1 ? "V":"N", pLhsOpt1 ? "V":"N", "N", NULL, &iCols, (doublecomplex*)pData1, &iCols, (doublecomplex*)pData2, &iCols
			 , &sDim, pAlpha, pBeta, (doublecomplex*)pLhsOpt1, &iCols, (doublecomplex*)pLhsOpt2, &iCols
			 , pCplxWork, &worksize, pRwork, pBwork, &info), info)
	  : -1;
	FREE(pAlpha);
	FREE(pBeta);
	break;
      }
    case 1+0: /* zgees */
      {
	doublecomplex* pW=NULL;
	info= ( (pCplxWork= iAllocZgeesWorkspace(iCols, pLhsOpt1 ? 1 :0, &worksize))
		&&(pRwork= (double*) MALLOC( iCols * sizeof(double)))
		&&(pW=(doublecomplex*)MALLOC(iCols * sizeof(doublecomplex))))
	  ? (C2F(zgees)(pLhsOpt1 ? "V":"N", "N", NULL, &iCols, (doublecomplex*)pData1, &iCols, &sDim, pW, (doublecomplex*)pLhsOpt1, &iCols
			, pCplxWork, &worksize, pRwork, pBwork, &info), info)
	  : -1;
	FREE(pW);
	break;
      }
    case 0+2: /* dgges */
      {
	double* pAlphaR= NULL;
	double* pAlphaI= NULL;
	double* pBeta= NULL;
	info= ( (pRwork= iAllocDggesWorkspace(iCols, pLhsOpt1 ? 1: 0, &worksize))
		&&(pAlphaI= (double*) MALLOC(iCols * sizeof(double)))
		&&(pAlphaR= (double*) MALLOC(iCols * sizeof(double)))
		&&(pBeta= (double*) MALLOC(iCols * sizeof(double))) )
	  ? ( C2F(dgges)(pLhsOpt1 ? "V":"N", pLhsOpt1 ? "V":"N", "N", NULL, &iCols, pData1, &iCols, pData2, &iCols, &sDim, pAlphaR, pAlphaI, pBeta
			 , pLhsOpt1, &iCols, pLhsOpt2, &iCols, pRwork, &worksize, pBwork, &info), info)
	  : -1;
	FREE(pAlphaR);
	FREE(pAlphaI);
	FREE(pBeta);
	break;
      }
    case 0+0: /* dgees */
      {
	double* pWR= NULL;
	double* pWI= NULL;
	info= ( (pRwork= iAllocDgeesWorkspace(iCols, pLhsOpt1 ? 1 : 0, &worksize))
		&&(pWR=(double*)MALLOC(iCols * sizeof(double)))
		&&(pWI=(double*)MALLOC(iCols * sizeof(double))))
	  ? (C2F(dgees)(pLhsOpt1 ? "V":"N", "N", NULL, &iCols, pData1, &iCols, &sDim, pWR, pWI, pLhsOpt1, &iCols, pRwork, &worksize, pBwork, &info), info)
	  : -1;
	FREE(pWR);
	FREE(pWI);
	break;
      }
    }
  FREE(pBwork);
  FREE(pRwork);
  FREE(pCplxWork);  
  return info;
}
