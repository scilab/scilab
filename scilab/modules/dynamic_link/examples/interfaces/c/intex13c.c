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

#include <string.h> 
#include <stdio.h>
#include "stack-c.h"

/** external functions to be called through this interface **/

static int f99 (double *,double *,int*, int *,int *,int *);

int intex13c(char *fname) 
{
  static int ierr;
  static int lr1,lc1,it1,m1, n1;
  static int minlhs=1, minrhs=1, maxlhs=1, maxrhs=1;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  GetRhsCVar(1,MATRIX_OF_DOUBLE_DATATYPE, &it1, &m1, &n1, &lr1,&lc1);
  
  f99(stk(lr1), stk(lc1), &it1, &m1, &n1, &ierr);
  
  if (ierr > 0) 
    {
      Scierror(999,"%s: Internal Error\n",fname);
      return 0;
    }
  LhsVar(1) = 1;
  return 0;
}

static int f99(double *ar,double *ac,int *ita,int *ma,int *na,int *err) 
{
  int i;
  *err=0;
  for ( i= 0 ; i < (*ma)*(*na) ; i++) ar[i] = 2*ar[i] ;
  if ( *ita == 1) 
    for ( i= 0 ; i < (*ma)*(*na) ; i++) ac[i] = 3*ac[i] ;
  return(0);
}
