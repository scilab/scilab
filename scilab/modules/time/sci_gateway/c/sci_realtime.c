/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#include "gw_time.h"
#include "realtime.h"
#include "api_common.h"
#include "api_double.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/ 
int sci_realtimeinit(char *fname,unsigned long fname_len)
{
  int m1 = 0,n1 = 0;
  int * p1_in_address = NULL;
  int res = 0;
  double * pDblReal = NULL;
  double zer=0.0;

  CheckRhs(1,1);
  CheckLhs(1,1);

  /*  checking variable scale */
  
  CheckScalar(1,m1,n1);

  getVarAddressFromPosition(1, &p1_in_address);
  res = getMatrixOfDouble(p1_in_address, &m1, &n1, &pDblReal);

  /* cross variable size checking */
  C2F(realtimeinit)(&zer,pDblReal);

  LhsVar(1) = 0;
  C2F(putlhsvar)();

  return 0;
}
/*--------------------------------------------------------------------------*/  
int sci_realtime(char *fname,unsigned long fname_len)
{
  int m1 = 0,n1 = 0;
  int * p1_in_address = NULL;
  int res = 0;
  double * pDblReal = NULL;

  CheckRhs(1,1);
  CheckLhs(1,1);

  /*  checking variable t */
  CheckScalar(1,m1,n1);

  getVarAddressFromPosition(1, &p1_in_address);
  res = getMatrixOfDouble(p1_in_address, &m1, &n1, &pDblReal);

  /* cross variable size checking */
  C2F(realtime)(pDblReal);

  LhsVar(1) = 0;
  C2F(putlhsvar)();

  return 0;
}               
/*--------------------------------------------------------------------------*/ 
