/* ==================================================================== */
/* Allan CORNET */
/* DIGITEO 2009 */
/* Template toolbox_skeleton */
/* This file is released into the public domain */
/* ==================================================================== */
#include "stack-c.h" 
#include "api_common.h"
#include "api_double.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "csub.h"
/* ==================================================================== */
int sci_csub(char *fname)
{
  
  int m1 = 0, n1 = 0;
  int *piAddressVarOne = NULL;
  double *pdVarOne = NULL;
  
  int m2 = 0, n2 = 0;
  int *piAddressVarTwo = NULL;
  double *pdVarTwo = NULL;
  
  int m_out = 0, n_out = 0;
  double dOut = 0.0;

  /* --> result = csub(3,8)
  /* check that we have only 2 parameters input */
  /* check that we have only 1 parameters output */
  CheckRhs(2,2) ;
  CheckLhs(1,1) ;   
  
  /* get Address of inputs */
  getVarAddressFromPosition(1, &piAddressVarOne);
  getVarAddressFromPosition(2, &piAddressVarTwo);
  
  /* check input type */
  if ( getVarType(piAddressVarOne) != sci_matrix )
  {
  	Scierror(999,"%s: Wrong type for input argument #%d: A scalar expected.\n",fname,1);
  	return 0;
  }
  
  if ( getVarType(piAddressVarTwo) != sci_matrix )
  {
  	Scierror(999,"%s: Wrong type for input argument #%d: A scalar expected.\n",fname,2);
  	return 0;
  }

  /* get matrix */
  getMatrixOfDouble(piAddressVarOne,&m1,&n1,&pdVarOne);
  getMatrixOfDouble(piAddressVarTwo,&m2,&n2,&pdVarTwo);
  
  /* check size */
  if ( (m1 != n1) && (n1 != 1) ) 
  {
  	Scierror(999,"%s: Wrong size for input argument #%d: A scalar expected.\n",fname,1);
  	return 0;
  }
  if ( (m2 != n2) && (n2 != 1) ) 
  {
  	Scierror(999,"%s: Wrong size for input argument #%d: A scalar expected.\n",fname,2);
  	return 0;
  }
  
  /* call c function csub */
  csub(&pdVarOne[0],&pdVarTwo[0],&dOut);
  
  /* create result on stack */
  m_out = 1;  n_out = 1;
  createMatrixOfDouble(Rhs + 1, m_out, n_out, &dOut);
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

