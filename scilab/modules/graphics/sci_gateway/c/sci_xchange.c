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
#include "GetProperty.h"
#include "axesScale.h"

#define VIEWING_RECT_SIZE 4

/*--------------------------------------------------------------------------*/
int sci_xchange( char * fname, unsigned long fname_len )
{
  int m1,n1,l1,m2,n2,l2,m3,n3,l3,l4,l5;
  int four = VIEWING_RECT_SIZE ;
  int one  = 1 ;
  int * xPixCoords;
  int * yPixCoords;
  double * xCoords;
  double * yCoords;
  int viewingRect[VIEWING_RECT_SIZE];

  CheckRhs(3,3);
  CheckLhs(1,3);

  GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);
  

  /* Convert coordinates */
  if ( strcmp(cstk(l3),"i2f") == 0) 
  {
    GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
    GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
		CheckSameDims(1,2,m1,n1,m2,n2);

    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l3);
    CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l4);
    /* Get rectangle */
    CreateVar(Rhs+3,MATRIX_OF_DOUBLE_DATATYPE,&one,&four,&l5);

    xPixCoords = istk(l1);
    yPixCoords = istk(l2);
    xCoords = stk(l3);
    yCoords = stk(l4);

    convertPixelCoordsToUserCoords(xPixCoords, yPixCoords,
                                   xCoords, yCoords,
                                   m1 * n1, viewingRect );

  }
  else 
  {
		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
		GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
		CheckSameDims(1,2,m1,n1,m2,n2);

    CreateVar(Rhs+1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l3);
    CreateVar(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l4);
    /* Get rectangle */
    CreateVar(Rhs+3,MATRIX_OF_DOUBLE_DATATYPE,&one,&four,&l5);
 
    xCoords = stk(l1);
    yCoords = stk(l2);
    xPixCoords = istk(l3);
    yPixCoords = istk(l4);

    convertUserCoordToPixelCoords(xCoords, yCoords,
                                  xPixCoords, yPixCoords,
                                  m1 * n1, viewingRect);
  }

  *stk(l5) = viewingRect[0];
  *stk(l5 + 1) = viewingRect[1];
  *stk(l5 + 2) = viewingRect[2];
  *stk(l5 + 3) = viewingRect[3];

  LhsVar(1)=Rhs+1;
  LhsVar(2)=Rhs+2;
  LhsVar(3)=Rhs+3;
	C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/

#undef VIEWING_RECT_SIZE

