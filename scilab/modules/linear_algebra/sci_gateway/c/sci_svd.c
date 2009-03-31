/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2009 - INRIA Bernard Hugueney
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
#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"

#include "svd.h"

/*
  s=svd(X): [R: min(rows, cols) x 1]
  [U,S,V]=svd(X) [U,S,V]:  [ [C|R: rows x rows], [R: rows x cols ],  [R|C: cols x cols ] ]
  [U,S,V]=svd(X,0) (obsolete) [U,S,V]=svd(X,"e"): [ [C|R:rows x min(rows,cols)], [R: min(rows,cols) x min(rows,cols)], [C|R:cols x min(rows,cols)] ]
  [U,S,V,rk]=svd(X [,tol]) : cf. supra, rk[R 1 x 1]
 */

extern int C2F(vfinite)(int *n, double *v);

int C2F(intsvd)(char *fname,unsigned long fname_len)
{
  int ret=0, economy=0, complexArg, iRows, iCols;
  double* pData= NULL;
  double* pDataReal= NULL;
  double* pDataImg= NULL;

  double* pSV= NULL;

  double* pU= NULL;
  double* pUReal= NULL;
  double* pUImg= NULL;

  double* pS= NULL;

  double* pV= NULL;
  double* pVReal= NULL;
  double* pVImg= NULL;

  double tol= 0.;
  double* pRk= NULL;

  if ( (Rhs >=1) && (GetType(1)!=sci_matrix))
    {
      OverLoad(1);
      return 0;
    }
  CheckRhs(1,2);
  CheckLhs(1, 4);

  economy= (Rhs==2) && (Lhs==3);
  if( (complexArg=iIsComplex(1)) )
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
  if(iRows == 0) /* empty matrix */
    {
      switch (Lhs){
	case 4:
	{
	  iAllocMatrixOfDouble(Rhs+5, 1,1, &pRk);
	  *pRk= 0.;
	  LhsVar(4)= Rhs + 5; /* no break */
	}
	case 3:
	{
	  double* dummy;
	  iAllocMatrixOfDouble(Rhs+3, iRows, iCols, &dummy); /* yes original Fortran code does this... (no check on iCols) */
	  LhsVar(3)= Rhs + 3; /* no break */
	}
	case 2: /* illegal according to doc, but there was Fortran code to handle this, so... */
	{
	  double* dummy;
	  iAllocMatrixOfDouble(Rhs+1, iRows, iCols, &dummy);
	  LhsVar(2)= Rhs + 1; /* no break */
	}
	case 1:
	{
	  LhsVar(1)= 1;
	}
      }
    }
  else
    {
      if((iRows == -1) || (iCols== -1))
	{
	  Scierror(999,_("Size varying argument a*eye(), (arg %d) not allowed here.\n"), 1);
	   ret= 1;
	}
      else
	{
	  int const totalsize= iRows * iCols * (complexArg ? 2 : 1); 
	  if(!C2F(vfinite)(&totalsize, pData))
	    {
	      Scierror(999,_("Wrong value for argument %d: Must not contain NaN or Inf.\n"), 1);
	      ret= 1;
	    }
	  else
	    {
	      switch(Lhs){
	      case 4:
		{
		  if( (Rhs == 2) && ( GetType(2) == sci_matrix) ) /* other cases with Rhs==2 are handled as economy mode */
		    {
		      int dummy; /* original code does not check iRows == iCols == 1 */
		      double* tmpData;
		      GetRhsVarMatrixDouble(2, &dummy, &dummy, &tmpData);
		      tol= *tmpData;
		    }
		  iAllocMatrixOfDouble(Rhs+4, 1, 1, &pRk);
		}/* no break */
	      case 3:
		{
		  int const economyRows= economy ? Min(iRows, iCols) : iRows;
		  int const economyCols= economy ? Min(iRows, iCols) : iCols;
		  if(complexArg)
		    {
		      iAllocComplexMatrixOfDouble(Rhs+1, iRows, economyRows , &pUReal, &pUImg);
		      pU= (double*)MALLOC(iRows * economyRows*(complexArg ? sizeof(doublecomplex): sizeof(double)));
		      iAllocMatrixOfDouble(Rhs+2, economyRows, economyCols, &pS);
		      iAllocComplexMatrixOfDouble(Rhs+3, iCols, economyCols , &pVReal, &pVImg);
		      pV= (double*)MALLOC(iCols * economyCols*(complexArg ? sizeof(doublecomplex): sizeof(double)));
		    }
		  else
		    {
		      iAllocMatrixOfDouble(Rhs+1, iRows, economyRows , &pU);
		      iAllocMatrixOfDouble(Rhs+2, economyRows, economyCols, &pS);
		      iAllocMatrixOfDouble(Rhs+3, iCols, economyCols , &pV);
		    }
		 
		  break;
		}
	      case 2:
		{
		  Scierror(999,_("%s: Wrong number of output arguments.\n"),fname);
		  break;
		}
	      case 1:
		{
		  iAllocMatrixOfDouble(Rhs+1, Min(iRows, iCols), 1, &pSV);
		  break;
		}
	      }
	      ret=  iSvdM(pData, iRows, iCols, complexArg, economy, tol, pSV, pU, pS, pV, pRk);
	      if(ret){
		if( ret == -1)
		  {
		    Scierror(999,_("%s: Cannot allocate more memory.\n"),fname);
		  }
		else
		  {
		    Scierror(999,_("Convergence problem...\n"));
		  }
	      }
	      else
		{
		  if(complexArg)
		    {
		      vFreeDoubleComplexFromPointer((doublecomplex*)pData);
		      if(Lhs != 1)
			{
			  {/* multicore: omp sections */
			    vGetPointerFromDoubleComplex((doublecomplex*)pU, iRows* (economy ? Min(iRows, iCols) : iRows), pUReal, pUImg);
			    FREE(pU);
			  }
			  {
			    vGetPointerFromDoubleComplex((doublecomplex*)pV, iCols* (economy ? Min(iRows, iCols) : iCols), pVReal, pVImg);
			    FREE(pV);
			  }
			}
		    }
		  switch(Lhs)
		    {
		    case 4: LhsVar(4)= Rhs + 4; /* no break */
		    case 3:
		      {
			LhsVar(3)= Rhs + 3;
			LhsVar(2)= Rhs + 2;
		      } /* no break */
		    case 1: LhsVar(1)= Rhs + 1 ;
		    }
		}
	    }
	}
    }
  return ret;
}
/*--------------------------------------------------------------------------*/
