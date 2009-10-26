/* ==================================================================== */
/* Allan CORNET */
/* DIGITEO 2009 */
/* Template toolbox_skeleton */
/* This file is released into the public domain */
/* ==================================================================== */
#include "stack-c.h" 
#include "api_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"
/* ==================================================================== */
extern int F2C(fsum)(double *a,double *b,double *c);
/* ==================================================================== */
int sci_fsum(char *fname)
{
  StrErr strErr;
  	
  int m1 = 0, n1 = 0;
  int *piAddressVarOne = NULL;
  double *pdVarOne = NULL;
  int iType1 = 0;  
  
  int m2 = 0, n2 = 0;
  int *piAddressVarTwo = NULL;
  double *pdVarTwo = NULL;
  int iType2 = 0;  
  
  int m_out = 0, n_out = 0;
  double dOut = 0.0;

  /* --> result = csum(3,8)
  /* check that we have only 2 parameters input */
  /* check that we have only 1 parameters output */
  CheckRhs(2,2) ;
  CheckLhs(1,1) ;   
  
  /* get Address of inputs */
  strErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
  if(strErr.iErr)
  {
    printError(&strErr, 0);
    return 0;
  }
    
  strErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
  if(strErr.iErr)
  {
    printError(&strErr, 0);
    return 0;
  }  
  
  /* check input type */
  strErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
  if(strErr.iErr)
  {
    printError(&strErr, 0);
    return 0;
  } 
   
  if ( iType1 != sci_matrix )
  {
    Scierror(999,"%s: Wrong type for input argument #%d: A scalar expected.\n",fname,1);
    return 0;
  }

  strErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
  if(strErr.iErr)
  {
    printError(&strErr, 0);
    return 0;
  }   
  
  if ( iType2 != sci_matrix )
  {
    Scierror(999,"%s: Wrong type for input argument #%d: A scalar expected.\n",fname,2);
    return 0;
  }

  /* get matrix */
  strErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne,&m1,&n1,&pdVarOne);
  if(strErr.iErr)
  {
    printError(&strErr, 0);
    return 0;
  }
    
  strErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo,&m2,&n2,&pdVarTwo);
  if(strErr.iErr)
  {
    printError(&strErr, 0);
    return 0;
  }  
  
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
  
  /* call fortran subroutine fsum */
  F2C(fsum)(&pdVarOne[0],&pdVarTwo[0],&dOut);
  
  /* create result on stack */
  m_out = 1;  n_out = 1;
  strErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m_out, n_out, &dOut);
  if(strErr.iErr)
  {
    printError(&strErr, 0);
    return 0;
  }  
  LhsVar(1) = Rhs + 1; 
  
  return 0;
}
/* ==================================================================== */


  