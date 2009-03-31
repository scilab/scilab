
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
#include "stack-c.h"
#include "stack3.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"

#include "rcond.h"

int C2F(intrcond)(char *fname,unsigned long fname_len)
{
  int ret= 0;
  /*   rcond(A)  */
  if ( (Rhs>=1) && (GetType(1)!=sci_matrix) )
    {
      OverLoad(1);
      return 0;
    }
  
  CheckRhs(1,1);
  CheckLhs(1,1);
  {
    double* pData;
    double* pDataReal;
    double* pDataImg;
    int iRows, iCols;
    int complexArg;
    if( (complexArg= iIsComplex(1)) )
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
	Scierror(999,_("%s: Wrong type for input argument #%d: Square matrix expected.\n"), fname, 1);
	ret= 1;
      }
    else
      {
	double* pRcond;
	int dim= iRows ? 1 : 0 ;
	iAllocMatrixOfDouble(2, dim, dim, &pRcond);
	if(iRows)
	  {
	    if( iRows == -1 )
	      {
		*pRcond= 1.;
	      }
	    else
	      {
		ret= iRcondM(pData, iCols, complexArg, pRcond);
	      }
	  }
	LhsVar(1)= 2;
      }
    if(complexArg)
      {
	 vFreeDoubleComplexFromPointer((doublecomplex*)pData);
      }
  }
  return ret;
}
