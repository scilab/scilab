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
/* file: sci_xchange.c                                                    */
/* desc : interface for xchange routine                                   */
/*------------------------------------------------------------------------*/

#include "sci_xchange.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "PloEch.h"

/*--------------------------------------------------------------------------*/
int sci_xchange( char * fname, unsigned long fname_len )
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,l4,l5,i;
  integer four = 4 ;
  integer one  = 1 ;
  CheckRhs(3,3);
  CheckLhs(1,3);

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
  GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);
  CheckSameDims(1,2,m1,n1,m2,n2);

  if ( strcmp(cstk(l3),"i2f") == 0) 
  {
    GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
    GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);

    CreateVar(Rhs+3,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l3);
    CreateVar(Rhs+4,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l4);

    C2F(echelle2d)(stk(l3),stk(l4),istk(l1),istk(l2),&m1,&n1,"i2f",3L);
  }
  else 
  {
    CreateVar(Rhs+3,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l3);
    CreateVar(Rhs+4,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l4);

    C2F(echelle2d)(stk(l1),stk(l2),istk(l3),istk(l4),&m1,&n1,"f2i",3L);
  }

  CreateVar(Rhs+5,MATRIX_OF_DOUBLE_DATATYPE,&one,&four,&l5);

  for (i=0; i < four ; i++) {*stk(l5+i) =  Cscale.WIRect1[i]; }
  LhsVar(1)=Rhs+3;
  LhsVar(2)=Rhs+4;
  LhsVar(3)=Rhs+5;
  return 0;
}
/*--------------------------------------------------------------------------*/
