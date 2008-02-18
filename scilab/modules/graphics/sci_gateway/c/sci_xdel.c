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
/* file: sci_xdel.c                                                       */
/* desc : interface for xdel routine                                      */
/*------------------------------------------------------------------------*/

#include "sci_xdel.h"
#include "stack-c.h"
#include "DestroyObjects.h"
#include "GetProperty.h"
#include "CurrentObjectsManagement.h"

/*--------------------------------------------------------------------------*/
int sci_xdel(char *fname,unsigned long fname_len)
{ 
  integer m1,n1,l1;
  CheckRhs(-1,1);
  if (Rhs >= 1) {
    int i;
    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1); 
    for ( i=0; i < m1*n1 ; i++ ) 
    {
      int win = (int) *stk(l1+i);
      sciDeleteWindow( win ) ;
    }
  } else {
    sciDeleteWindow( sciGetNum(sciGetCurrentFigure()) ) ;
  }
  LhsVar(1)=0;
  return 0;
} 

/*--------------------------------------------------------------------------*/
