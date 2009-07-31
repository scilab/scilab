
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
  int* arg= NULL;

  if ( Rhs>=1 )
    {
      getVarAddressFromPosition(1, &arg);
      if( getVarType(arg)!=sci_matrix )
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
	if( (complexArg= isVarComplex(arg)) )
	  {
	    getComplexZMatrixOfDouble(arg, &iRows, &iCols, (doublecomplex**)&pData);
	  }
	else
	  {
	    getMatrixOfDouble(arg, &iRows, &iCols, &pData);
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
		if( (ret= 
		     (complexArg
		      ? allocComplexMatrixOfDouble(2, 1, 1, &pMantissaReal, &pMantissaImg)
		      : allocMatrixOfDouble(2, 1, 1, &pMantissaReal))
		     ||( (Lhs == 2) && allocMatrixOfDouble(3, 1, 1, &pExponent)))
		    )
		  {
		    Scierror(999,_("%s: stack size exceeded (Use stacksize function to increase it).\n"), fname);
		  }
		else
		  {
		    int intExponent;
		    ret= iDetM(pData, iCols, pMantissaReal, complexArg ? pMantissaImg : NULL, pExponent ? &intExponent : NULL);
		    if(pExponent)
		      {
			*pExponent= (double)intExponent;
		      }
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
