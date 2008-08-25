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
#include "GetProperty.h"
#include "ObjectSelection.h"
#include "WindowList.h"
#include "axesScale.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_xgetmouse( char *fname,unsigned long fname_len )
{
  integer  m1=1,n1=3,l1,l2;
  int mouseButtonNumber = 0;
  int windowsID = 0;
  integer sel[2],m,n;

  int pixelCoords[2];
  double userCoords2D[2];
  sciPointObj * clickedSubwin = NULL;

  int displayWarning = FALSE;
  int selPosition = 0;

  CheckRhs(0,2);
  CheckLhs(1,2);

  // Select current figure or create it
  sciGetCurrentFigure();

  switch(Rhs)
    {
    case 1:
      if (GetType(1)==sci_matrix)
        {
          displayWarning = TRUE;
        }
      else if (GetType(1)==sci_boolean)
        {
          selPosition = 1;
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: Single double or boolean vector expected.\n"), fname, 1);
          return FALSE;
        }
      break;
    case 2:
      if (GetType(1)==sci_matrix)
        {
          displayWarning = TRUE;
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: Single double expected.\n"), fname, 1);
          return FALSE;
        }

      if (GetType(2)==sci_boolean)
        {
          selPosition = 2;
        }
      else
        {
          Scierror(999, _("%s: Wrong type for input argument #%d: Boolean vector expected.\n"), fname, 2);
          return FALSE;
        }
      break;
    default:
      // Call Java xgetmouse
      // No need to set any option.
      break;
    }


  // Display a warning if a flag has been given (obsolete use)
  if (displayWarning)
    {
      sciprint(_("%s: Using xgetmouse with a flag to avoid the event queue to be cleared is obsolete.\nThis functionnality will be permanently removed in Scilab 5.1.\n"),fname);
    }

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
    return 0;
  case 2:
    LhsVar(1) = Rhs+1;

    CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&m1,&l2);
    *stk(l2) = windowsID; /* this is the window number */
    LhsVar(2) = Rhs+2;
    return 0;
  }
  return -1 ;
}

/*--------------------------------------------------------------------------*/
