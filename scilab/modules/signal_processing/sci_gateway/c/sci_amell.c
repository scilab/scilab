/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - INRIA - Vincent LIARD
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "gw_signal.h"
#include "stack-c.h"
/****************************************************************/
extern int C2F(amell)(double *du, double *dk, double *dsn2, int *n);
/****************************************************************/
int sci_amell(char *fname,unsigned long fname_len)
{
  int r1, nrows1, ncols1, r2, nrows2, ncols2;

  double *pdblReal1 = NULL;
  double *pdblReal2 = NULL;
  double *pdblRealOut = NULL;
  int iSizeOut = 0;

  CheckRhs(2,2);
  CheckLhs(1,1);
  
  GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &nrows1, &ncols1, &r1);
  pdblReal1 = stk(r1);
  GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &nrows2, &ncols2, &r2);
  pdblReal2 = stk(r2);
  // TODO: fix type checking
  // 1st parameter must be a real number
  if (iIsComplex(1))
    {
      Err = 1;
      Error(52);
      return 1;
    }
  // 2nd parameter must be a real number
  if (iIsComplex(2))
    {
      Err = 2;
      Error(52);
      return 1;
    }
  // 2nd parameter must be scalar-typed
  if (nrows2 != 1 || ncols2 != 1)
    {
      Error(84);
      return 1;
    }
  // 2nd parameter in [0,1]
  // TODO: transfer to amell.f (since this is an application code constraint)
  if (*pdblReal2 < 0 || *pdblReal2 > 1)
    {
      Error(36);
      return 1;
    }

  iAllocMatrixOfDouble(Rhs + 1, nrows1, ncols1, &pdblRealOut);
  iSizeOut = nrows1 * ncols1;
  C2F(amell)(pdblReal1, pdblReal2, pdblRealOut, &iSizeOut);
  LhsVar(1) = Rhs + 1;
  PutLhsVar();
  return 0;
}
/****************************************************************/
