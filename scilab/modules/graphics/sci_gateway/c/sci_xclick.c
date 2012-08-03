/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xclick.c                                                     */
/* desc : interface for xclick routine                                    */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "CallJxclick.h"
#include "GetProperty.h"
#include "FigureList.h"
#include "axesScale.h"
#include "HandleManagement.h"
#include "BuildObjects.h"
#include "CurrentSubwin.h"

#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"

/*--------------------------------------------------------------------------*/
int sci_xclick(char *fname,unsigned long fname_len)
{
  int one = 1, three = 3, rep = 0;
  int istr = 0;
  //int iflag = 0;

  int mouseButtonNumber = 0;
  char * menuCallback = NULL;
  char *pstWindowUID = NULL;
  int pixelCoords[2];
  double userCoords2D[2];

  int iFigureId = 0;
  int *piFigureId = &iFigureId;

  CheckRhs(-1, 1) ;
  CheckLhs(1, 5) ;

  //iflag = ( Rhs >= 1) ? 1 :0;

  // Select current figure or create it
  getOrCreateDefaultSubwin();

  // Call Java xclick
  CallJxclick();

  // Get return values
  mouseButtonNumber = getJxclickMouseButtonNumber();
  pixelCoords[0] = (int) getJxclickXCoordinate();
  pixelCoords[1] = (int) getJxclickYCoordinate();
  pstWindowUID = getJxclickWindowID();
  menuCallback = getJxclickMenuCallback();

  // Convert pixel coordinates to user coordinates
  // Conversion is not done if the user clicked on a menu (pixelCoords[*] == -1)
  if (pixelCoords[0] != -1 && pixelCoords[1] != -1)
  {
    char* clickedSubwinUID = (char*)getCurrentSubWin();
    sciGet2dViewCoordFromPixel(clickedSubwinUID, pixelCoords, userCoords2D);
  }
  else
  {
    userCoords2D[0] = pixelCoords[0];
    userCoords2D[1] = pixelCoords[1];
  }

  if (Lhs == 1)
  {
    LhsVar(1) = Rhs+1;
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&one,&three,&rep);
    *stk(rep) = (double) mouseButtonNumber;
    *stk(rep + 1) = userCoords2D[0];
    *stk(rep + 2) = userCoords2D[1];
  }
  else
  {
    LhsVar(1) = Rhs+1;
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&one,&one,&rep);
    *stk(rep) = (double) mouseButtonNumber;
  }

  if (Lhs >= 2)
  {
    LhsVar(2) = Rhs+2;
    CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&one,&one,&rep);
    *stk(rep) = userCoords2D[0];
  }

  if (Lhs >= 3)
  {
    LhsVar(3) = Rhs+3;
    CreateVar(Rhs+3,MATRIX_OF_DOUBLE_DATATYPE,&one,&one,&rep);
    *stk(rep) = userCoords2D[1];
  }

  if (Lhs >=4)
  {
    LhsVar(4) = Rhs+4;
    CreateVar(Rhs+4,MATRIX_OF_DOUBLE_DATATYPE,&one,&one,&rep);
    getGraphicObjectProperty(pstWindowUID, __GO_ID__, jni_int, (void**)&piFigureId);
    *stk(rep) = (double) iFigureId;
  }

  if (Lhs >= 5)
  {
    LhsVar(5) = Rhs+5;
    istr = (int)strlen(menuCallback);
    CreateVar(Rhs+5,STRING_DATATYPE,&istr,&one,&rep);
    strncpy(cstk(rep),menuCallback,istr);
  }

  deleteJxclickString(menuCallback);
  deleteJxclickString(pstWindowUID);

  PutLhsVar();

  return 0;
}
/*--------------------------------------------------------------------------*/
