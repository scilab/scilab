/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Clément DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "business.h"

#include "stack-c.h"
#include "api_scilab.h"
#include "Scierror.h"

int sci_tbx_sum(char *fname) {
  SciErr sciErr;
  
  int *piAddressVarOne = NULL;
  double dVarOne = 0.0;
  
  int *piAddressVarTwo = NULL;
  double dVarTwo = 0.0;
  
  double dOut = 0.0;
  
  /* check that we have only 2 input arguments */
  /* check that we have only 1 output argument */
  CheckRhs(2,2);
  CheckLhs(1,1);
  
    /* get Address of inputs */
  sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
  if(sciErr.iErr)
  {
    printError(&sciErr, 0);
    return 0;
  }
  
  sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
  if(sciErr.iErr)
  {
    printError(&sciErr, 0);
    return 0;
  }
  
  /* check input type */
  if ( !isDoubleType(pvApiCtx, piAddressVarOne) )
  {
    Scierror(999,"%s: Wrong type for input argument #%d: A scalar expected.\n", fname, 1);
    return 0;
  }
  
  if ( !isDoubleType(pvApiCtx, piAddressVarTwo) )
  {
    Scierror(999,"%s: Wrong type for input argument #%d: A scalar expected.\n", fname, 2);
    return 0;
  }

  if ( getScalarDouble(pvApiCtx, piAddressVarOne, &dVarOne) )
  {
    Scierror(999,"%s: Wrong size for input argument #%d: A scalar expected.\n", fname, 1);
    return 0;
  }

  if ( getScalarDouble(pvApiCtx, piAddressVarTwo, &dVarTwo) )
  {
    Scierror(999,"%s: Wrong size for input argument #%d: A scalar expected.\n", fname, 2);
    return 0;
  }
  
  /* call c business function */  
  dOut = business_sum(dVarOne, dVarTwo);
  
  /* create result on stack */
  createScalarDouble(pvApiCtx, Rhs + 1, dOut);
  LhsVar(1) = Rhs + 1; 
}

