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
/* file: sci_rotate_axes.c                                                */
/* desc : interface for rotate_axes routine                               */
/*------------------------------------------------------------------------*/

#include "sci_rotate_axes.h"
#include "stack-c.h"
#include "Scierror.h"
#include "Interaction.h"
#include "localization.h"
#include "HandleManagement.h"
#include "getPropertyAssignedValue.h"
#include "returnProperty.h"
#include "GetProperty.h"
#include "CurrentObjectsManagement.h"

/*--------------------------------------------------------------------------*/
int sci_rotate_axes(char *fname,unsigned long fname_len)
{ 
  /* call rotate_axes(handle) where handle */
  /* is a figure or subwin handle */

  sciPointObj * rotatedObject = NULL;


  int nbRow;
  int nbCol;
  size_t stackPointer = 0;

  /* check size of input and output */
  CheckRhs(0,1);
  CheckLhs(0,1);

  if (Rhs == 0)
  {
    /* rotate current figure */
    interactiveRotation(sciGetCurrentFigure());
  }
  else
  {
    /* Get figure or subwin handle */
    if (GetType(1) != sci_handles)
    {
      Scierror(999, _("%s: Wrong type for input argument #%d: Single Figure or Axes handle expected.\n"), fname, 1);
      LhsVar(1) = 0;
	  C2F(putlhsvar)();
      return -1;
    }

    GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stackPointer);

    if (nbRow * nbCol != 1)
    {
      Scierror(999, _("%s: Wrong type for input argument #%d: Single Figure or Axes handle expected.\n"), fname, 1);
      LhsVar(1) = 0;
	  C2F(putlhsvar)();
      return -1;
    }

    rotatedObject = sciGetPointerFromHandle(getHandleFromStack(stackPointer));

	if (rotatedObject == NULL)
	{
		Scierror(999,_("%s: The handle is not or no more valid.\n"),fname);
        return -1;
	}
    else if (sciGetEntityType(rotatedObject) == SCI_FIGURE)
    {
      interactiveRotation(rotatedObject);
    }
    else if (sciGetEntityType(rotatedObject) == SCI_SUBWIN)
    {
      interactiveSubwinRotation(rotatedObject);
    }
    else
    {
      Scierror(999, _("%s: Wrong type for input argument #%d: Single Figure or Axes handle expected.\n"), fname, 1);
      return -1;
    }
  }

  LhsVar(1) = 0;
  C2F(putlhsvar)();

  return 0;
}
/*--------------------------------------------------------------------------*/
