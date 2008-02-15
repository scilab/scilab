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

/****************************************
 *     an example with  GetMatrixptr 
 *     -->param=[1,2,3];         
 *     -->link('ext8c.o','ext8c','C');     
 *     -->y=call('ext8c','out',size(param),1,'d');
 ****************************************/

int ext8c(double *y)
{
  static int m, n, lp,i;
  GetMatrixptr("param", &m, &n, &lp);
  /* param can be changed */
  *stk(lp)=18.0;
  /* param can be read */
  for (i=0; i < m*n ; i++ ) y[i] = (*stk(lp+i));
  return 0;
}
