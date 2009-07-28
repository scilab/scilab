/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2007-2008 - INRIA - Vincent Couvert
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xgetmouse.c                                                  */
/* desc : interface for sci_xgetmouse routine                             */
/*------------------------------------------------------------------------*/

#include "sci_xgetmouse.h"
#include "stack-c.h"
#include "GetProperty.h" /* sciGetNum */
#include "CurrentObjectsManagement.h" /* sciGetCurrentFigure */
#include "CallJxgetmouse.h"
#include "ObjectSelection.h"
#include "WindowList.h"
#include "axesScale.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_xgetmouse( char *fname,unsigned long fname_len )
{
  int  m1=1,n1=3,l1,l2;
  int mouseButtonNumber = 0;
  int windowsID = 0;
  int sel[2],m,n;

  int pixelCoords[2];
  double userCoords2D[2];
  sciPointObj * clickedSubwin = NULL;

  int displayWarning = FALSE;
  int selPosition = 0;

  CheckRhs(0,1);
  CheckLhs(1,2);

  switch(Rhs)
    {
    case 1:
      if (GetType(1)==sci_boolean)
        {
          selPosition = 1;
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: Boolean vector expected.\n"), fname, 1);
          return FALSE;
        }
      break;
    default:
      // Call Java xgetmouse
      // No need to set any option.
      break;
    }

  // Select current figure or create it
  sciGetCurrentFigure();

  // Call Java to get mouse information
  if (selPosition!=0)
    {
      GetRhsVar(selPosition,MATRIX_OF_BOOLEAN_DATATYPE, &m, &n, &l1);
      CheckDims(selPosition,m*n,1,2,1);
      sel[0]=*istk(l1);
      sel[1]=*istk(l1+1);

      // Call Java xgetmouse
      CallJxgetmouseWithOptions(sel[0], sel[1]);
    }
  else
    {
      CallJxgetmouse();
    }

  // Get return values
  mouseButtonNumber = getJxgetmouseMouseButtonNumber();
  pixelCoords[0] = (int) getJxgetmouseXCoordinate();
  pixelCoords[1] = (int) getJxgetmouseYCoordinate();
  windowsID = (int )getJxgetmouseWindowsID();

  CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
  // No need to calculate coordinates if callback or close is trapped
  if (mouseButtonNumber == -1000 || mouseButtonNumber == -2)
    {
      *stk(l1) = -1;
      *stk(l1+1) = -1;
      *stk(l1+2) = (double) mouseButtonNumber;
    }
  else
    {
      // Convert pixel coordinates to user coordinates
      clickedSubwin = sciGetFirstTypedSelectedSon(getFigureFromIndex(windowsID), SCI_SUBWIN);
      updateSubwinScale(clickedSubwin);
      sciGet2dViewCoordFromPixel(clickedSubwin, pixelCoords, userCoords2D);
      *stk(l1) = userCoords2D[0];
      *stk(l1+1) = userCoords2D[1];
      *stk(l1+2) = (double) mouseButtonNumber;
    }
  LhsVar(1) = Rhs+1;

  switch (Lhs) {
  case 1:
		C2F(putlhsvar)();
    return 0;
  case 2:
    CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&m1,&l2);
    *stk(l2) = windowsID; /* this is the window number */
    LhsVar(2) = Rhs+2;
		C2F(putlhsvar)();
    return 0;
  }
	C2F(putlhsvar)();
  return -1 ;
}

/*--------------------------------------------------------------------------*/
