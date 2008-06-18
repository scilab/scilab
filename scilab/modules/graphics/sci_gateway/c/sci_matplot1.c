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
/* file: sci_matplot1.c                                                   */
/* desc : interface for matplot1 routine                                  */
/*------------------------------------------------------------------------*/

#include "sci_matplot.h"
#include "GetProperty.h"
#include "sci_demo.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "sciCall.h"

/*--------------------------------------------------------------------------*/
int sci_matplot1( char * fname, unsigned long fname_len )
{
  integer m1, n1, l1,m2,n2,l2;
  if ( Rhs <= 0 )
  {
    int zero = 0 ;
    sci_demo(fname,"plot2d([0,10],[0,10],0);a=ones(50,50);a= 3*tril(a)+2*a;Matplot1(a,[4,4,9,9]);", &zero);
    return 0;
  }

  CheckRhs(2,2);
  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
  CheckLength(2,m2*n2,4);
  if ( m1 * n1 == 0 )
  {
    LhsVar(1) = 0 ;
    return 0 ;
  } 

  /* NG beg */

  Objmatplot1 (stk(l1), &m1, &n1,stk(l2)) ;

  LhsVar(1)=0;
  return 0;
} 
/*--------------------------------------------------------------------------*/
