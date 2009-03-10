
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

#include <string.h>
#include "stack-c.h"
#include "MALLOC.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"
#include "lu.h"

#include "stack2.h" /* to create eye matrix */

int C2F(intlu)(char *fname,unsigned long fname_len)
{
  int complexArg;
  int ret;
  int iRows, iCols;
  double* pData;
  double* pDataReal;
  double* pDataImg;
  
  /*   lu(A)  */
  if ( (Rhs >=1 ) && GetType(1)!=sci_matrix) 
    {
      OverLoad(1);
      return 0;
    }
  CheckRhs(1,1); /* one and only one arg */
  CheckLhs(2,3); /* [L,U,[E]] = lu(A) */
  complexArg=iIsComplex(1);
  if(complexArg)
    {
      GetRhsVarMatrixComplex(1, &iRows, &iCols, &pDataReal, &pDataImg);
      /* c -> z */
      pData=(double*)oGetDoubleComplexFromPointer( pDataReal, pDataImg, iRows * iCols);
    }
	else
	  {
	    GetRhsVarMatrixDouble(1, &iRows, &iCols, &pData);
	  }
	if( (iCols == 0) || (iRows == 0))
	  {
	    double* pdblL= NULL;
	    LhsVar(1)= 1;
	    ret = iAllocMatrixOfDouble(2, 0, 0, &pdblL);
	    /* check pbdlL or ret */
	    LhsVar(2)= 2;
	    if(Lhs == 3)
	      {
		double* pdblE= NULL;
		ret = iAllocMatrixOfDouble(2, 0, 0, &pdblE);
		/* check pbdlE or ret */
		LhsVar(3)= 3;
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
		    iAllocComplexMatrixOfDouble(2, -1, -1, &pdblLReal, &pdblLImg);
		    *pdblLReal= *pDataReal;
		    *pdblLImg= *pDataImg;
		    *pDataReal= 1.;
		    *pDataImg= 0.;
		  }
		else
		  {
		    double* pdblLData;
		    iAllocMatrixOfDouble(2, -1, -1, &pdblLData);
		    *pdblLData= *pData;
		    *pData= 1.;
		}
		LhsVar(2)= 2;
		if(Lhs == 3)
		  {
		    if(complexArg)
		      {
			double* pdblEReal;
			double* pdblEImg;
			iAllocComplexMatrixOfDouble(3, -1, -1, &pdblEReal, &pdblEImg);
			*pdblEReal= 1.;
			*pdblEImg= 0.;
		      }
		    else
		      {
			double* pdblEData;
			iAllocMatrixOfDouble(3, -1, -1, &pdblEData);
			*pdblEData= 1.;
		      }
		    LhsVar(3)= 3;
		  }
	      }
	    else
	      {
		double *pdblLData;
		double *pdblLReal;
		double *pdblLImg;
		double *pdblUData;
		double *pdblUReal;
		double *pdblUImg;
		double *pdblEData;
		int iMinRowsCols;

		pdblEData= NULL;
		iMinRowsCols= Min(iRows, iCols);

		if(complexArg)
		  {
		    iAllocComplexMatrixOfDouble(2, iRows, iMinRowsCols, &pdblLReal, &pdblLImg);
		    iAllocComplexMatrixOfDouble(3, iMinRowsCols, iCols, &pdblUReal, &pdblUImg);
		    /*
		      we can allocate matrix of 'z' instead of calling oGetDoubleComplexFromPointer because the freshly allocated
		      complex matrix does not contain any useful data.
		     */
		    pdblLData = (double*)MALLOC(iRows * iMinRowsCols * sizeof(doublecomplex) );
		    pdblUData = (double*)MALLOC(iMinRowsCols * iCols * sizeof(doublecomplex) );
		  }
		else
		  {
		    iAllocMatrixOfDouble(2, iRows, iMinRowsCols, &pdblLData);
		    iAllocMatrixOfDouble(3, iMinRowsCols, iCols, &pdblUData);
		  }
		if(Lhs == 3)
		  {
		    iAllocMatrixOfDouble(4, iRows, iRows, &pdblEData);
		  }
		ret =iLuM(pData, iRows, iCols, complexArg, pdblLData, pdblUData, pdblEData );
		if(complexArg)
		  {
		    vGetPointerFromDoubleComplex((doublecomplex*)pdblLData, iRows * iMinRowsCols, pdblLReal, pdblLImg);
		    FREE(pdblLData);
		    vGetPointerFromDoubleComplex((doublecomplex*)pdblUData, iMinRowsCols * iCols, pdblUReal, pdblUImg);
		    FREE(pdblUData);
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
	return 0;
}
/*--------------------------------------------------------------------------*/
