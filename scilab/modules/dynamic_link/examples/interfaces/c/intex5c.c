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

#include <stdlib.h>
#include "stack-c.h"

int crestrc (char **a, int *m,int *err);

/* Creating a scilab variable from a pointer
 * l1 is a pointer to a char array created 
 * by the C function crestrc (in file pgmsc.c)   
 */

int intex5c(char* fname)
{ 
  char *l1;
  int m1,n1,ierr=0;
  static int minlhs=1, minrhs=0, maxlhs=1, maxrhs=0;

  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);

  crestrc( &l1,&m1,&ierr);
  if ( ierr > 0 ) 
    {
      Scierror(999,"%s: Internal error\n",fname);
      return 0;
    }

  n1=1;
  CreateVarFromPtr( 1,STRING_DATATYPE, &m1, &n1, &l1);
  free(l1);
  LhsVar(1) = 1;
  return(0);
}
