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
#include "matmul.h"

/*--------------------------------------------------------
 *  Matrix multiplication C= A*B, (A,B,C stored columnwise) 
 *  C function
 *--------------------------------------------------------*/

#define A(i,k) a[i + k*n]
#define B(k,j) b[k + j*m]
#define C(i,j) c[i + j*n]

void matmul(double a[],int n,int m,double b[],int l, double c[])
{
  int i = 0,j = 0,k = 0;
  double s = 0.0;
  
  for( i=0 ; i < n; i++)
  {
    for( j=0; j < l; j++)
    {
      s = 0.;
      for( k=0; k< m; k++)
      {
	      s += A(i,k)*B(k,j);
      }
      C(i,j) = s;
    }
  }
}
