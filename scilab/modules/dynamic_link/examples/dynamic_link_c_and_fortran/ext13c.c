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


struct {
    float b[10];
} cmntest;

#define Max(x,y)        (((x)>(y))?(x):(y))

int ext13ic(int *n, float *a)
{
  int i1;
  static int k;
  i1 = Max(*n,10);
  for (k = 0; k < i1; ++k) {
    cmntest.b[k] = a[k];
  }
} 

int ext13oc(int *n, float *c)
{
  int i1;
  static int k;
  i1 = Max(*n,10);
  for (k = 0; k < i1; ++k) {
    c[k] = cmntest.b[k];
  }
} 


