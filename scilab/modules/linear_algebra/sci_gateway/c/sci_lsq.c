
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
#include "api_common.h"
#include "api_double.h"

#include <string.h>
#include <stdio.h>
#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "stack3.h"
#include "localization.h"
#include "MALLOC.h"

#include "lsq.h"

/*
 * Implementation note: this is a faithful translation of the original Fortran codes. 
 * It could be made more efficient be directly retrieving the rhs data in
 * a sufficienlty large matrix (to be used by Lapack). Currently, we do 
 * 1) get rhs data to rows x nRhs matrix
 * 2) copy it to max(rows, cols) x nRhs buffer matrix and pass it to Lapack
 * 3) copy result from buffer matrix to cols x nRhs result matrix
 * unnecessary copies could be avoided especially in the complex case.
 *
 */
#define STACK3
/*--------------------------------------------------------------------------*/
extern int C2F(complexify)(int *num);
/*--------------------------------------------------------------------------*/
int C2F(intlsq)(char *fname,unsigned long fname_len)
{
  int* arg[3]= {NULL, NULL, NULL};
  int ret= 0;
  if (Rhs>=1)
    {
      getVarAddressFromPosition(1, &arg[0]);
      if(getVarType(arg[0])!=sci_matrix)
	{
	  OverLoad(1);
	  return 0;
	}
      if (Rhs >= 2)
	{
	  getVarAddressFromPosition(2, &arg[1]);
	  if(getVarType(arg[1])!=sci_matrix)
	    {
	      OverLoad(2);
	      return 0;
	    }
	}
      {
	double* pData[2]= {NULL, NULL};
	double* pDataReal[2]= {NULL, NULL};
	double* pDataImg[2]= {NULL, NULL};
	int iCols[2]= {0, 0}, iRows[2]= {0, 0};
	int complexArgs= 0;
	int i;
	double* pTol=NULL;
	
	CheckRhs(2,3); /* lsq(A, B [,tol]) */
	CheckLhs(1, 2);/* [ X [,rank] ] */
	
	complexArgs= isVarComplex(arg[0]) || isVarComplex(arg[1]);
	for(i=0; i != 2 ; ++i)
	  {
	    int iPlus1= i+1;
	    if(complexArgs)
	      {
		C2F(complexify)(&iPlus1);
#ifdef STACK3
		GetRhsVarMatrixComplex(iPlus1, &iRows[i], &iCols[i], &pDataReal[i], &pDataImg[i]);
#else
		getComplexMatrixOfDouble(arg[i], &iRows[i], &iCols[i], &pDataReal[i], &pDataImg[i]);
#endif
		pData[i]= iCols[i] ? (double*)oGetDoubleComplexFromPointer( pDataReal[i], pDataImg[i], iRows[i] * iCols[i]) : NULL;
	      }
	    else
	      {
#ifdef STACK3
		GetRhsVarMatrixDouble(iPlus1, &iRows[i], &iCols[i], &pData[i]);
#else
		getMatrixOfDouble(arg[i], &iRows[i], &iCols[i], &pData[i]);
#endif
	      }
	    if(iCols[i] == -1)
	      {
		Scierror(271, _("Size varying argument a*eye(), (arg %d) not allowed here.\n"),iPlus1);
		ret= 1;
	      }
	    if( (i==1) && (iRows[0] != iRows[1]) )
	      {
		Scierror(265, _("%s and %s must have equal number of rows.\n"), "A", "B");
		ret= 1;
	      }
	  }
	if(Rhs == 3)
	  {
	    getVarAddressFromPosition(3, &arg[2]);
	    if(getVarType(arg[2]) != sci_matrix)
	      { /* original fortran implementation does not warn */
		Scierror(256, _("%s: Wrong type for input argument #%d: A Real expected.\n"),fname, 3);
		ret= 1;
	      }
	    else
	      {
		int unused;
#ifdef STACK3
		GetRhsVarMatrixDouble(3, &unused, &unused, &pTol);
#else
		getMatrixOfDouble(arg[2],&unused, &unused, &pTol);
#endif
	      }
	  }
	if(!ret)
	  {
	    double* pResult= NULL;
	    double* pResultReal= NULL;
	    double* pResultImg= NULL;
	    int rank;
	    int emptyMatrix = (iRows[0]== 0) || (iCols[0]==0);

	    ret= complexArgs 
	      ? ( pResult= (emptyMatrix ? NULL : (double*)MALLOC( iCols[0] * iCols[1] * sizeof(doublecomplex)))) /* MALLOC(0) is invalid :( */
	      ,allocComplexMatrixOfDouble(Rhs+1, iCols[0], iCols[1] , &pResultReal, &pResultImg)
	      : allocMatrixOfDouble(Rhs+1, iCols[0], iCols[1], &pResult) ;
	    if(ret==0)
	      {
		ret= emptyMatrix ? 0 : iLsqM(pData[0], iRows[0], iCols[0], pData[1], iCols[1], complexArgs, pResult, pTol, (Lhs==2) ? &rank : NULL);

		if(complexArgs)
		  {
		    for(i=0; i != 2; ++i)
		      {
			vFreeDoubleComplexFromPointer((doublecomplex*)pData[i]);
		      }
		    vGetPointerFromDoubleComplex((doublecomplex*)(pResult), iCols[0] * iCols[1], pResultReal, pResultImg);
		    FREE(pResult);
		  }
		LhsVar(1)= Rhs + 1;
		if(Lhs == 2)
		  {
		    double* pRank;
		    if(emptyMatrix)
		      {
			allocMatrixOfDouble(Rhs+2, 0, 0, &pRank);
		      }
		    else
		      {
			allocMatrixOfDouble(Rhs+2, 1, 1, &pRank);
			*pRank= (double) rank;
		      }
		    LhsVar(2)= Rhs + 2;
		  }
	      }
	    else
	      {
		Scierror(999,_("%s: Cannot allocate more memory.\n"),fname);
		ret= -1;
	      }
	  }
      }
    }
  return ret;
}
