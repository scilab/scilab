
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
#include "stack-c.h"
#include "MALLOC.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"
#include "lu.h"

#include "stack2.h" /* to create eye matrix */

#define STACK3

int C2F(intlu)(char *fname,unsigned long fname_len)
{
  int* arg= NULL;
  int complexArg;
  int ret;
  int iRows, iCols;
  double* pData;
  double* pDataReal;
  double* pDataImg;

  ret= 0;
  
  /*   lu(A)  */
  if ( Rhs >=1 )
    {
      getVarAddressFromPosition(1,&arg);
      if(getVarType(arg)!=sci_matrix) 
	{
	  OverLoad(1);
	  return 0;
	}
    
      CheckRhs(1,1); /* one and only one arg */
      CheckLhs(2,3); /* [L,U,[E]] = lu(A) */
      complexArg=isVarComplex(arg);
      if(complexArg)
	{
#ifdef STACK3
	  GetRhsVarMatrixComplex(1, &iRows, &iCols, &pDataReal, &pDataImg);
#else
	  getComplexMatrixOfDouble(arg, &iRows, &iCols, &pDataReal, &pDataImg);
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
	  getMatrixOfDouble(arg, &iRows, &iCols, &pData);
#endif

	}
      if( (iCols == 0) || (iRows == 0))
	{
	  double* pdblL= NULL;
	  LhsVar(1)= 1;
	  if((ret = allocMatrixOfDouble(2, 0, 0, &pdblL)))
	    {
	      Scierror(999,_("%s: stack size exceeded (Use stacksize function to increase it).\n"), fname);
	    }
	  else
	    {
	      LhsVar(2)= 2;
	    }
	  if(Lhs == 3)
	    {
	      double* pdblE= NULL;
	      if((ret = allocMatrixOfDouble(3, 0, 0, &pdblE)))
		{
		  Scierror(999,_("%s: stack size exceeded (Use stacksize function to increase it).\n"), fname);
		}
	      else
		{
		  LhsVar(3)= 3;
		}
	    }
	}
      else
	{
	  if( (iCols == -1) && (iRows == -1)) /* Rhs(1)=k*eye() => Lhs(1)=eye() Lhs(2)=k*eye(), Lhs(3)=eye() */
	    {
	      LhsVar(1)= 1;
	      if(complexArg)
		{
		  double* pdblLReal;
		  double* pdblLImg;
		  if((ret = allocComplexMatrixOfDouble(2, -1, -1, &pdblLReal, &pdblLImg)))
		    {
		      Scierror(999,_("%s: stack size exceeded (Use stacksize function to increase it).\n"), fname);
		    }
		  else
		    {
		      *pdblLReal= *pDataReal;
		      *pdblLImg= *pDataImg;
		      *pDataReal= 1.;
		      *pDataImg= 0.;
		    }
		}
	      else
		{
		  double* pdblLData;
		  if((ret = allocMatrixOfDouble(2, -1, -1, &pdblLData)))
		    {
		      Scierror(999,_("%s: stack size exceeded (Use stacksize function to increase it).\n"), fname);
		    }
		  else
		    {
		      *pdblLData= *pData;
		      *pData= 1.;
		    }
		}
	      LhsVar(2)= 2;
	      if(Lhs == 3)
		{
		  if(complexArg)
		    {
		      double* pdblEReal;
		      double* pdblEImg;
		      if((ret= allocComplexMatrixOfDouble(3, -1, -1, &pdblEReal, &pdblEImg)))
			{
			  Scierror(999,_("%s: stack size exceeded (Use stacksize function to increase it).\n"), fname);
			}
		      else
			{
			  *pdblEReal= 1.;
			  *pdblEImg= 0.;
			}
		    }
		  else
		    {
		      double* pdblEData;
		      if((ret=  allocMatrixOfDouble(3, -1, -1, &pdblEData)))
			{
			  Scierror(999,_("%s: stack size exceeded (Use stacksize function to increase it).\n"), fname);
			}
		      else
			{
			  *pdblEData= 1.;
			}
		    }
		}
	      LhsVar(3)= 3;
	    }
	  else
	    {
	      double *pdblLData= NULL;
	      double *pdblLReal= NULL;
	      double *pdblLImg= NULL;
	      double *pdblUData= NULL;
	      double *pdblUReal= NULL;
	      double *pdblUImg= NULL;
	      double *pdblEData= NULL;
	      int iMinRowsCols;
	      
	      pdblEData= NULL;
	      iMinRowsCols= Min(iRows, iCols);
	  
	      if(complexArg)
		{
		  if(allocComplexMatrixOfDouble(2, iRows, iMinRowsCols, &pdblLReal, &pdblLImg)
		     || allocComplexMatrixOfDouble(3, iMinRowsCols, iCols, &pdblUReal, &pdblUImg))
		    {
		      Scierror(999,_("%s: stack size exceeded (Use stacksize function to increase it).\n"), fname);
		      ret= 1;
		    }
		  /*
		    we can allocate matrix of 'z' instead of calling oGetDoubleComplexFromPointer because the freshly allocated
		    complex matrix does not contain any useful data.
		  */
		  pdblLData = (double*)MALLOC(iRows * iMinRowsCols * sizeof(doublecomplex) );
		  if(!pdblLData)
		    {
		      Scierror(999,_("%s: Cannot allocate more memory.\n"),fname);
		      ret = 1;
		    }
		  else
		    {
		      pdblUData = (double*)MALLOC(iMinRowsCols * iCols * sizeof(doublecomplex) );
		      if(!pdblUData)
			{
			  Scierror(999,_("%s: Cannot allocate more memory.\n"),fname);
			  ret=1;
			}
		    }
		}
	      else
		{
		  if(allocMatrixOfDouble(2, iRows, iMinRowsCols, &pdblLData)
		     ||allocMatrixOfDouble(3, iMinRowsCols, iCols, &pdblUData))
		    {
		      Scierror(999,_("%s: stack size exceeded (Use stacksize function to increase it).\n"), fname);
		      ret= 1;
		    }
		}
	      if(Lhs == 3)
		{
		  if(allocMatrixOfDouble(4, iRows, iRows, &pdblEData))
		    {
		      Scierror(999,_("%s: stack size exceeded (Use stacksize function to increase it).\n"), fname);
		      ret=1;
		    }
		}
	      /* using ?: short circuit to avoid calling function if an alloc went wrong */
	      ret = ret ? ret : iLuM(pData, iRows, iCols, complexArg, pdblLData, pdblUData, pdblEData );
	      if(complexArg)
		{
		  if(pdblLData)
		    {
		      vGetPointerFromDoubleComplex((doublecomplex*)pdblLData, iRows * iMinRowsCols, pdblLReal, pdblLImg);
		      FREE(pdblLData);
		    }
		  if(pdblUData)
		    {
		      vGetPointerFromDoubleComplex((doublecomplex*)pdblUData, iMinRowsCols * iCols, pdblUReal, pdblUImg);
		      FREE(pdblUData);
		    }
		}
	      LhsVar(1)= 2;
	      LhsVar(2)= 3;
	      if(Lhs == 3)
		{
		  LhsVar(3)= 4;
		}
	    }
	}
      /* TODO rajouter le PutLhsVar(); quand il sera enlevé du gw_ */
    }
  return 0;
}
/*--------------------------------------------------------------------------*/
