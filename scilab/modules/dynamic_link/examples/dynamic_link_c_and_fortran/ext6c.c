/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA/ENPC
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "stack-c.h"

/******************************************
 *   example 6 
 *   reading a vector in scilab internal stack using ReadMatrix 
 *     -->link('ext6c.o','ext6c','C') 
 *     -->a=[1,2,3];b=[2,3,4];
 *     -->c=call('ext6c',a,1,'c',b,2,'d','out',[1,3],3,'d') 
 *     -->c=a+2*b 
 ******************************************/

int ext6c(char *aname, double *b, double *c)
{   
  double a[3];
  int k, m, n;
  ReadMatrix(aname, &m, &n, a);
  /*     [m,n]=size(a)  here m=1 n=3  */
  for (k = 0; k < n; ++k) 
    c[k] = a[k] + b[k] * 2.;
  return(0);
}
