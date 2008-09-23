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
/* file: sci_xtring.c                                                     */
/* desc : interface for xstring routine                                   */
/*------------------------------------------------------------------------*/

#include "sci_xstring.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "sciCall.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_xstring( char *fname, unsigned long fname_len )
{
  double rect[4],x,y,yi,angle=0.0;
  int flagx=0;
  int m1,n1,l1,m2,n2,l2,m3,n3,m4,n4,l4,m5,n5,l5;
  char **Str = NULL;
  long hdlstr ;
  BOOL isboxed = FALSE;

  CheckRhs(3,5);

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
  CheckScalar(1,m1,n1);
  x = *stk(l1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  yi = y = *stk(l2);
  GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&m3,&n3,&Str);
  if ( m3*n3 == 0 ) { LhsVar(1)=0; return 0;} 

  if (Rhs >= 4)
  {
    GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE,&m4,&n4,&l4);
    CheckScalar(4,m4,n4);
    angle = DEG2RAD(*stk(l4));
  }
  if (Rhs >= 5)
  { 
    GetRhsVar(5,MATRIX_OF_DOUBLE_DATATYPE,&m5,&n5,&l5);
    CheckScalar(5,m5,n5);
    flagx = (int)  *stk(l5);
  }


  if ( (flagx == 1) && (*stk(l4) == 0))
  {
    isboxed = TRUE;
  }

  /* create the object */
  Objstring ( Str,m3,n3,x,y,&angle,rect,TRUE,NULL,&hdlstr,FALSE,NULL,NULL,isboxed,TRUE,FALSE, ALIGN_LEFT ) ;


  /* we must free Str memory */ 
  
  freeArrayOfString(Str,m3*n3);

  LhsVar(1)=0;
  return 0;
} 
/*--------------------------------------------------------------------------*/
