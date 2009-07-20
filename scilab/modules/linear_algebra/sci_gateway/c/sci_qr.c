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
#include "api_common.h"
#include "api_double.h"

#include <string.h>
#include <stdio.h>
#include "stack3.h"
#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
#include "qr.h"


#include <stdio.h>

/* conditional allocs short-circuits :
   MALLOC :
   0 -> error,  !=0 -> ok
   (MALLOC1 && MALLOC2 &&...) : 0-> error, !=0 ok
   (!Cond1 || MALLOC1) && (!Cond2 || MALLOC2) &&

   iAlloc :
   0 -> ok, , !0 -> error
   (iAlloc1 || iAlloc2 ...)
   (Cond1 && iAlloc1) || (Cond2 && iAlloc2) ...
   
*/
/*


iQrM(pData, iRows, iCols, complexArg, iRowsToCompute, tol, pdblQ, pdblR, pdblE, pdblRank)

*/
/*--------------------------------------------------------------------------*/
int C2F(intqr)(char *fname,unsigned long fname_len)
{
  int* arg[2]= {NULL, NULL};
  
  int iRows, iCols;
  double* pData= NULL;
  double* pDataReal= NULL;
  double* pDataImg= NULL;
  int complexArg= 0;
  int ret= 0;

  if (Rhs>=1)
    {
      getVarAddressFromPosition(1, &arg[0]);
      if( getVarType(arg[0])!=sci_matrix)
	{
	  OverLoad(1);
	  return 0;
	}
      CheckRhs(1,2); /* qr(X[,"e"|tol]) */
      CheckLhs(1,4); /*[Q,R[,E]]=qr(X[,"e"]), [Q,R,rk,E]=qr(X[,tol])*/
      complexArg= isVarComplex(arg[0]);
      
      if(complexArg)
	{
#ifdef STACK3
	  GetRhsVarMatrixComplex(1, &iRows, &iCols, &pDataReal, &pDataImg);
#else
	  getComplexMatrixOfDouble(arg[0], &iRows, &iCols, &pDataReal, &pDataImg);
#endif
	  /* c -> z */
	  pData=(double*)oGetDoubleComplexFromPointer( pDataReal, pDataImg, iRows * iCols);
	  if(!pData)
	    {
	      Scierror(999,_("%s: Cannot allocate more memory.\n"),fname);
	      ret = 1;
	    }
	}
      else
	{
#ifdef STACK3
	  GetRhsVarMatrixDouble(1, &iRows, &iCols, &pData);
#else
	  getMatrixOfDouble(arg[0], &iRows, &iCols, &pData);
#endif
	}
      if( (iCols == 0) || (iRows == 0))
	{
	  if( complexArg )
	    {
	      double* pdblQReal;
	      double* pdblQImg;
	      double* pdblRReal;
	      double* pdblRImg;
	      allocComplexMatrixOfDouble(Rhs+1, 0, 0, &pdblQReal, &pdblQImg);
	      allocComplexMatrixOfDouble(Rhs+2, 0, 0, &pdblRReal, &pdblRImg);	  
	    }
	  else
	    {
	      double* pdblQ;
	      double* pdblR;
	      allocMatrixOfDouble(Rhs+1, 0, 0, &pdblQ);
	      allocMatrixOfDouble(Rhs+2, 0, 0, &pdblR);
	    }
	  LhsVar(1)= Rhs+1;
	  LhsVar(2)= Rhs+2;
	  if( Lhs >= 3) /* ..[rk],E] =*/
	    {
	      double* pdblE;
	      allocMatrixOfDouble(Rhs+3, 0, 0, &pdblE);
	      if( Lhs == 4 ) /* rk */
		{
		  double* pdblRk;
		  allocMatrixOfDouble(Rhs+4, 1, 1, &pdblRk);
		  *pdblRk= 0.;
		  LhsVar(3)= Rhs+4;
		  LhsVar(4)= Rhs+3;
		}
	      else
		{
		  LhsVar(3)= Rhs+3;
		}
	    }
	}
      else
	{
	  if( (iRows == -1) || (iCols == -1) )
	    {
	      Scierror(999,_("Size varying argument a*eye(), (arg %d) not allowed here.\n"), 1);
	      ret= 1;
	    }
	  else /* now at last the interesting case */
	    {
	      int iRowsToCompute= iRows;
	      double dblTol= -1.;
	      if(Rhs == 2) /* first check economy mode or tolerance */
		{
		  getVarAddressFromPosition(2, &arg[1]);
		  switch( getVarType(arg[1])){
		  case sci_strings :
		    { /* /!\ original code did not check that string is "e" so any [matrix of] string is accepted as "e" ! */
		      iRowsToCompute= Min(iRows, iCols);
		      break;
		    }
		  case sci_matrix :
		    {/* /!\ original code do not check anything (real && 1x1 matrix)*/
		      double* pdblTol;
		      int tmpRows, tmpCols;
#ifdef STACK3
		      GetRhsVarMatrixDouble(2, &tmpRows, &tmpCols, &pdblTol);
#else
		      getMatrixOfDouble(arg[1], &tmpRows, &tmpCols, &pdblTol);
#endif
		      dblTol= *pdblTol;
		      break;
		    }
		  default:
		    {
		      Scierror(999,_("%s: Wrong type for input argument #%d: A real or a string expected.\n"),fname, 1);
		      ret= 1;
		      break;
		    }
		  }
		}
	      {
		double* pdblQ= NULL;
		double* pdblQReal= NULL;
		double* pdblQImg= NULL;
		double* pdblR= NULL;
		double* pdblRReal= NULL;
		double* pdblRImg= NULL;
		double* pdblE= NULL;
		double* pdblRk= NULL;
	    
		if(complexArg)
		  {
		    if( (ret= (allocComplexMatrixOfDouble(Rhs+1, iRows, iRowsToCompute, &pdblQReal, &pdblQImg)
			       ||allocComplexMatrixOfDouble(Rhs+2, iRowsToCompute, iCols, &pdblRReal, &pdblRImg))))
		      {
			Scierror(999,_("%s: stack size exceeded (Use stacksize function to increase it).\n"), fname);
		      }
		    else
		      {
			if( ( ret= !((pdblQ= (double*) MALLOC(iRows * iRowsToCompute * sizeof(doublecomplex) ))
				     &&(pdblR= (double*) MALLOC( iRowsToCompute * iCols * sizeof(doublecomplex) )))))
			  {
			    Scierror(999,_("%s: Cannot allocate more memory.\n"),fname);
			  }
		      }
		  }
		else 
		  {
		    if( (ret= (allocMatrixOfDouble(Rhs+1, iRows, iRowsToCompute, &pdblQ)
			       ||allocMatrixOfDouble(Rhs+2, iRowsToCompute, iCols, &pdblR))))
		      {
			Scierror(999,_("%s: stack size exceeded (Use stacksize function to increase it).\n"), fname);
		      }
		  }
		if( !ret ){
		  if( (ret=  
		       (( Lhs >= 3) /* next alloc for E needed only for lhs>=3 */
			&& allocMatrixOfDouble(Rhs+3, iCols, iCols, &pdblE)
			)
		       ||((Lhs >=4) /* next alloc for Rk needed only for lhs>=4 */
			  && allocMatrixOfDouble(Rhs+4, 1, 1, &pdblRk))
		       ))
		    {
		      Scierror(999,_("%s: stack size exceeded (Use stacksize function to increase it).\n"), fname);
		    }
		}
		ret = ret ? ret : iQrM(pData, iRows, iCols, complexArg, iRowsToCompute, dblTol, pdblQ, pdblR, pdblE, pdblRk);
		if( complexArg )
		  {
		    if(pdblQ)
		      {
			vGetPointerFromDoubleComplex((doublecomplex*)pdblQ, iRows * iRowsToCompute, pdblQReal, pdblQImg);
			FREE(pdblQ);
		      } 
		    if(pdblR)
		      {
			vGetPointerFromDoubleComplex((doublecomplex*)pdblR, iRowsToCompute * iCols, pdblRReal, pdblRImg);
			FREE(pdblR);
		      }
		  }
		LhsVar(1)= Rhs+1;
		LhsVar(2)= Rhs+2;
		if(Lhs >= 3)
		  {
		    if(Lhs == 4)
		      {
			LhsVar(3)= Rhs+4;
			LhsVar(4)= Rhs+3;
		      }
		    else
		      {
			LhsVar(3)= Rhs+3;
		      }
		  }
	      }
	    }
	}
      /* TODO PutLhsVar(); when removed from gw */
    }
  return ret;
}
/*--------------------------------------------------------------------------*/
