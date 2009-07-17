/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xgraduate.c                                                  */
/* desc : interface for xgraduate routine                                 */
/*------------------------------------------------------------------------*/

#include "sci_xgraduate.h"
#include "stack-c.h"
#include "Format.h"

/*--------------------------------------------------------------------------*/
int sci_xgraduate(char *fname,unsigned long fname_len)
{
  double xa,xi;
  int m1,n1,l1,m2,n2,l2,i;
  int kMinr,kMaxr,ar,lr,np1,np2,un=1;

  CheckRhs(2,2);
  CheckLhs(2,7);
  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);  CheckScalar(1,m1,n1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);  CheckScalar(2,m2,n2);

  C2F(graduate)(stk(l1),stk(l2),&xi,&xa,&np1,&np2,&kMinr,&kMaxr,&ar);

  *stk(l1) = xi;
  *stk(l2) = xa;

  if (Lhs >= 3) { CreateVar(3,MATRIX_OF_DOUBLE_DATATYPE,&un,&un,&lr); *stk(lr ) = (double) np1;  }
  if (Lhs >= 4) { CreateVar(4,MATRIX_OF_DOUBLE_DATATYPE,&un,&un,&lr); *stk(lr ) = (double) np2;  }
  if (Lhs >= 5) { CreateVar(5,MATRIX_OF_DOUBLE_DATATYPE,&un,&un,&lr); *stk(lr ) = (double) kMinr;  }
  if (Lhs >= 6) { CreateVar(6,MATRIX_OF_DOUBLE_DATATYPE,&un,&un,&lr); *stk(lr ) = (double) kMaxr;  }
  if (Lhs >= 7) { CreateVar(7,MATRIX_OF_DOUBLE_DATATYPE,&un,&un,&lr); *stk(lr ) = (double) ar;  }
  for (i= 1; i <= Lhs ; i++) { LhsVar(i) = i ; }
	C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/
