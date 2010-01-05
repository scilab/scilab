
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
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
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"
#include "doublecomplex.h"

#include "det.h"

/*
 * det function see http://www.scilab.org/product/man/det.html
 * det(X)
 * [e,m]=det(X)
 */

int C2F(intdet)(char *fname,unsigned long fname_len)
{
  int ret= 0;
  int type=0;
  int* arg= NULL;

  if ( Rhs>=1 )
    {
      getVarAddressFromPosition(pvApiCtx, 1, &arg);
      getVarType(pvApiCtx, arg, &type);
      if( type!=sci_matrix )
	{
	  OverLoad(1);
	  return 0;
	}
      CheckRhs(1, 1);
      CheckLhs(1, 2);
      {
	double* pData;
	int iRows, iCols;
	int complexArg;
	if( (complexArg= isVarComplex(pvApiCtx, arg)) )
	  {
	    getComplexZMatrixOfDouble(pvApiCtx, arg, &iRows, &iCols, (doublecomplex**)&pData);
	  }
	else
	  {
	    getMatrixOfDouble(pvApiCtx, arg, &iRows, &iCols, &pData);
	  }
	if( iRows != iCols)
	  {
	    Scierror(20,_("%s: Wrong type for input argument #%d: Square matrix expected.\n"), fname, 1);
	    ret= 1;
	  }
	else
	  {
	    if(iRows == -1)
	      {
		Scierror(271,_("Size varying argument a*eye(), (arg %d) not allowed here.\n"), 1);
		ret= 1;
	      }
	    else
	      {
		double* pMantissaReal= NULL;
		double* pMantissaImg= NULL;
		double* pExponent= NULL;
		if(complexArg)
		{
			allocComplexMatrixOfDouble(pvApiCtx, 2, 1, 1, &pMantissaReal, &pMantissaImg);
		}
		else
		{
			allocMatrixOfDouble(pvApiCtx, 2, 1, 1, &pMantissaReal);
		}

		if(Lhs == 2)
		{
			allocMatrixOfDouble(pvApiCtx, 3, 1, 1, &pExponent);
		}

		int intExponent;
		ret= iDetM(pData, iCols, pMantissaReal, complexArg ? pMantissaImg : NULL, pExponent ? &intExponent : NULL);
		if(pExponent)
		{
			*pExponent= (double)intExponent;
		}
		LhsVar(1)= Lhs + 1;
		if(Lhs == 2)
		  {
		    LhsVar(2)= 2;
		  }
	      }
	  }
	if(complexArg)
	  {
	    vFreeDoubleComplexFromPointer((doublecomplex*)pData);
	  }
      }
    }
  return ret;
}
