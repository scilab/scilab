/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2009 - INRIA Bernard HUGUENEY
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
  int iRows, iCols;
  double* pData= NULL;
  double* pDataReal= NULL;
  double* pDataImg= NULL;
  int complexArg= 0;
  int ret= 0;

  if ( (Rhs>=1) && (GetType(1)!=sci_matrix) )
    {
      OverLoad(1);
      return 0;
    }
  CheckRhs(1,2); /* qr(X[,"e"|tol]) */
  CheckLhs(1,4); /*[Q,R[,E]]=qr(X[,"e"]), [Q,R,rk,E]=qr(X[,tol])*/
  complexArg= iIsComplex(1);

  if(complexArg)
    {
      GetRhsVarMatrixComplex(1, &iRows, &iCols, &pDataReal, &pDataImg);
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
      GetRhsVarMatrixDouble(1, &iRows, &iCols, &pData);
    }
  if( (iCols == 0) || (iRows == 0))
    {
      if( complexArg )
	{
	  double* pdblQReal;
	  double* pdblQImg;
	  double* pdblRReal;
	  double* pdblRImg;
	  iAllocComplexMatrixOfDouble(Rhs+1, 0, 0, &pdblQReal, &pdblQImg);
	  iAllocComplexMatrixOfDouble(Rhs+2, 0, 0, &pdblRReal, &pdblRImg);	  
	}
      else
	{
	  double* pdblQ;
	  double* pdblR;
	  iAllocMatrixOfDouble(Rhs+1, 0, 0, &pdblQ);
	  iAllocMatrixOfDouble(Rhs+2, 0, 0, &pdblR);
	}
      LhsVar(1)= Rhs+1;
      LhsVar(2)= Rhs+2;
      if( Lhs >= 3) /* ..[rk],E] =*/
	{
	  double* pdblE;
	  iAllocMatrixOfDouble(Rhs+3, 0, 0, &pdblE);
	  if( Lhs == 4 ) /* rk */
	    {
	      double* pdblRk;
	      iAllocMatrixOfDouble(Rhs+4, 1, 1, &pdblRk);
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
	      switch( GetType(2)){
	      case sci_strings :
		{ /* /!\ original code did not check that string is "e" so any [matrix of] string is accepted as "e" ! */
		  iRowsToCompute= Min(iRows, iCols);
		  break;
		}
	      case sci_matrix :
		{/* /!\ original code do not check anything (real && 1x1 matrix)*/
		  double* pdblTol;
		  int tmpRows, tmpCols;
		  GetRhsVarMatrixDouble(2, &tmpRows, &tmpCols, &pdblTol);
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
		if( (ret= (iAllocComplexMatrixOfDouble(Rhs+1, iRows, iRowsToCompute, &pdblQReal, &pdblQImg)
			   ||iAllocComplexMatrixOfDouble(Rhs+2, iRowsToCompute, iCols, &pdblRReal, &pdblRImg))))
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
		if( (ret= (iAllocMatrixOfDouble(Rhs+1, iRows, iRowsToCompute, &pdblQ)
			   ||iAllocMatrixOfDouble(Rhs+2, iRowsToCompute, iCols, &pdblR))))
		  {
		     Scierror(999,_("%s: stack size exceeded (Use stacksize function to increase it).\n"), fname);
		  }
	      }
	    if( !ret ){
	      if( (ret=  
		   (( Lhs >= 3) /* next alloc for E needed only for lhs>=3 */
		    && iAllocMatrixOfDouble(Rhs+3, iCols, iCols, &pdblE)
		    )
		   ||((Lhs >=4) /* next alloc for Rk needed only for lhs>=4 */
		      && iAllocMatrixOfDouble(Rhs+4, 1, 1, &pdblRk))
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
  return ret;
}
/*--------------------------------------------------------------------------*/
