
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
#include "localization.h"
#include "stack3.h"
#include "MALLOC.h"
#include "hess.h"

int C2F(inthess)(char *fname,unsigned long fname_len)
{
  int iRows, iCols;
  double* pData= NULL;
  double* pDataReal= NULL;
  double* pDataImg= NULL;
  int complexArg= 0;
  int ret= 0;
  /*   hess(A)  */
  if ( (Rhs >= 1) && (GetType(1)!=sci_matrix) )
    {
      OverLoad(1);
      return 0;
    }
  CheckRhs(1,1); /* H[,U]= hess(A) */
  CheckLhs(1,2);
  
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
  if( iRows != iCols)
    {
      Scierror(999,_("%s: Wrong size for input argument #%d: Square matrix expected.\n"),fname, 1);
    }
  else
    {
      if( iCols == 0)
	{
	  LhsVar(1)= 1;
	  if(Lhs == 2)
	    {
	      double* unused;
	      if(complexArg)
		{
		  iAllocComplexMatrixOfDouble(2, 0, 0, &unused, &unused);
		}
	      else
		{
		  iAllocMatrixOfDouble(2, 0, 0, &unused);
		}
	      LhsVar(2)= 2;
	    }
	}
      else
	{
	  if(iCols == -1 )
	    {
	      if( Lhs == 1 )
		{
		  LhsVar(1)= 1;
		}
	      else
		{
		  if(complexArg)
		    {
		      double* pHReal= NULL;
		      double* pHImg= NULL;
		      iAllocComplexMatrixOfDouble(2, -1, -1, &pHReal, &pHImg);
		      *pHReal= *pDataReal;
		      *pHImg= *pDataImg;
		      *pDataReal= 1.;
		      *pDataImg= 0.;
		    }
		  else
		    {
		      double* pH= NULL;
		      iAllocMatrixOfDouble(2, -1, -1, &pH);
		      *pH= *pData;
		      *pData= 1.;
		    }
		  LhsVar(1)= 1;
		  LhsVar(2)= 2;
		}
	    }
	  else
	    { /* neither empty nor eye matrix: at last the interesting case ! */
	      double* pH= NULL;
	      double* pHReal= NULL;
	      double* pHImg= NULL;
	      if(Lhs == 2)
		{ /* perform memory allocation for second Lhs (H) */
		  if(complexArg)
		    {
		      if( (ret= iAllocComplexMatrixOfDouble(2, iRows, iCols, &pHReal, &pHImg)) )
			{
			  Scierror(999,_("%s: stack size exceeded (Use stacksize function to increase it).\n"), fname);
			}
		      else
			{
			  if( ( ret= !(pH= (double*) MALLOC(iRows * iCols * sizeof(doublecomplex) ) ) ) )
			    {
			      Scierror(999,_("%s: Cannot allocate more memory.\n"),fname);
			    }
			}
		    }
		  else 
		    {
		      if( (ret= iAllocMatrixOfDouble(2, iRows, iCols, &pH)) )
			{
			  Scierror(999,_("%s: stack size exceeded (Use stacksize function to increase it).\n"), fname);
			}
		    }
		}
	      ret = ret ? ret : iHessM(pData, iCols, complexArg, pH);
	      if(complexArg)
		{
		  vGetPointerFromDoubleComplex((doublecomplex*)pData, iRows * iCols, pDataReal, pDataImg);
		  FREE(pData);
		  if(pH)
		    {
		      vGetPointerFromDoubleComplex((doublecomplex*)pH, iRows * iCols, pHReal, pHImg);
		      FREE(pH);
		    }
		}
	      if(Lhs == 1)
		{
		  LhsVar(1)= 1;
		}
	      else
		{
		  LhsVar(1)= 2;
		  LhsVar(2)= 1;
		}
	    }
	}
    }
  return ret;
}      
