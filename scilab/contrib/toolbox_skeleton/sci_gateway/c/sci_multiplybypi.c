/* ==================================================================== */
/* 2011 - Digiteo - Sylvestre LEDRU
/* Template toolbox_skeleton */
/* This file is released under the 3-clause BSD license. See COPYING-BSD. */
/* ==================================================================== */
#include "stack-c.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "multiplybypi.h"
/* ==================================================================== */
int sci_multiplybypi(char *fname)
{
  SciErr sciErr;
  
  int m1 = 0, n1 = 0;
  int *piAddressVarOne = NULL;
  double *matrixOfDouble = NULL;
  int iType1 = 0;
  int i = 0;

  /* --> result = multiplybypi(8) */
  /* --> result = multiplybypi([12, 42; 42, 12]) */
  /* check that we have only 1 input argument */
  /* check that we have only 1 output argument */
  CheckRhs(1,1) ;
  CheckLhs(1,1) ;   
  
  /* get Address of inputs */
  sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
  if(sciErr.iErr)
  {
    printError(&sciErr, 0);
    return 0;
  }
    

  /* check input type */
  sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
  if(sciErr.iErr)
  {
    printError(&sciErr, 0);
    return 0;
  }
  
  if ( iType1 != sci_matrix )
  {
  	Scierror(999,"%s: Wrong type for input argument #%d: A matrix expected.\n",fname,1);
  	return 0;
  }
  
  /* get matrix */
  sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &m1, &n1, &matrixOfDouble);
  if(sciErr.iErr)
  {
    printError(&sciErr, 0);
    return 0;
  }
  
  /* The difference with the csum & csub is that we give the argument as copy
   * and not as reference */
  for (i=0; i < m1*n1; i++) {
      /* For each element of the matrix, multiply by pi */
      matrixOfDouble[i] = multiplybypi(matrixOfDouble[i]);
  }

  /* Create the matrix as return of the function */
  createMatrixOfDouble(pvApiCtx, Rhs + 1, m1, n1, matrixOfDouble);
  if(sciErr.iErr)
  {
      printError(&sciErr, 0);
      return 0;
  }

  LhsVar(1) = Rhs + 1; 
  
  /* This function put on scilab stack, the lhs variable
  which are at the position lhs(i) on calling stack */
  /* You need to add PutLhsVar here because WITHOUT_ADD_PUTLHSVAR 
  was defined and equal to %t */
  /* without this, you do not need to add PutLhsVar here */
  PutLhsVar();
  
  return 0;
}
/* ==================================================================== */

