/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 * 
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 */

#include "stack-c.h"

/** external functions to be called through this interface **/

int as2osc (char *thechain);

/*--------------------------------------------------------
 * A string argument passed to as2osc
 *--------------------------------------------------------*/

int intex9c(char* fname)
{ 
  int l1,m1,n1;
  static int minlhs=1, minrhs=1, maxlhs=1, maxrhs=1;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  GetRhsVar( 1,STRING_DATATYPE, &m1, &n1, &l1);

  as2osc(cstk(l1));

  LhsVar(1) = 1;
  return(0);
}

