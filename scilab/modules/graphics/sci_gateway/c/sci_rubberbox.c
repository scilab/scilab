/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_rubberbox.c                                                  */
/* desc : interface for rubberbox routine                                 */
/*------------------------------------------------------------------------*/
#include "sci_rubberbox.h"
#include "returnProperty.h"
#include "stack-c.h"
#include "localization.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "Interaction.h"
#include "CurrentObjectsManagement.h"

/*--------------------------------------------------------------------------*/
static int getInitialRectangle(double initRect[4]);
static BOOL getEditionMode(void);
static int returnRectAndButton(const double selectedRect[4], int button);
/*--------------------------------------------------------------------------*/
static int getInitialRectangle(double initRect[4])
{
  int rectNbRow = 0;
  int rectNbCol = 0;
  int rectStackPointer = 0;

  /* Rhs is 0 or 1 */
  if (Rhs == 1 && GetType(1) == sci_matrix)
  {
    /* intial rect specified, not edition mode */
    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&rectNbRow,&rectNbCol,&rectStackPointer);
  }

  if (rectStackPointer != 0)
  {
    /* intial rect has been found */
    int i;

    /* pointer on the stack */
    double * rect = getDoubleMatrixFromStack(rectStackPointer);

    /* We have found a matrix of double within the parameters check its dims */
    int nbDims = rectNbCol * rectNbRow;
    if (nbDims != 2 && nbDims != 4)
    {
      sciprint(_("%s: Wrong size for input argument #%d: Vector of size %d or %d expected.\n"), "rubberbox", 1, 2, 4);
      return -1;
    }

    /* intialize to 0 */
    for (i = 0; i < 4; i++)
    {
      initRect[i] = 0.0;
    }

    /* Set specified values */
    for (i = 0; i < nbDims; i++)
    {
      initRect[i] = rect[i];
    }

    /* initial rect was specified */
    return 1;
  }

  /* initial rect was not specified */
  return 0;
}
/*--------------------------------------------------------------------------*/
static BOOL getEditionMode(void)
{
  if (Rhs == 1 && GetType(1) == sci_boolean)
  {
    return TRUE;
  }
  return FALSE;
}
/*--------------------------------------------------------------------------*/
static int returnRectAndButton(const double selectedRect[4], int button)
{
  int selectedRectSize = 4;
  int one = 1;
  int rectStackPointer = 0;
  int i;

  /* return rectangle */
  CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &one, &selectedRectSize, &rectStackPointer);
  for ( i = 0; i < selectedRectSize; i++)
  {
    stk(rectStackPointer)[i] = selectedRect[i];
  }
  LhsVar(1) = Rhs + 1;
  
  /* return button */
  if (Lhs >= 2)
  {
    int buttonStackPointer = 0;
    CreateVar(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &buttonStackPointer);
    *stk(buttonStackPointer) = button;
    LhsVar(2) = Rhs + 2;
  }

  return 0;

}
/*--------------------------------------------------------------------------*/
int sci_rubberbox(char * fname, unsigned long fname_len)
{
  
  /* [final_rect, btn] = rubberbox([edition_mode, initial_rect]) */


  /* Default values, intial rect and edition mode to false */
  double initialRect[4] = {0.0, 0.0, 0.0, 0.0};
  int initialRectSpecified = 0;
  int button = 0;
  double selectedRect[4];

  CheckRhs(0,1);
  CheckLhs(1,2);

  /* Get intial rect if one */
  initialRectSpecified = getInitialRectangle(initialRect);
  if (initialRectSpecified == 1 )
  {
    /* edition mode is false if initial rect is specified */
    rubberBox(sciGetCurrentSubWin(), TRUE, FALSE, initialRect, selectedRect, &button);
  }
  else if (initialRectSpecified == 0)
  {
    /* no intial rect */
    if (getEditionMode())
    {
      rubberBox(sciGetCurrentSubWin(), FALSE, FALSE, NULL, selectedRect, &button);
    }
    else
    {
      /* no initial rect and no edition mode */
      if (Rhs == 0)
      {
        /* ok no parameter at all */
        rubberBox(sciGetCurrentSubWin(), TRUE, FALSE, NULL, selectedRect, &button);
      }
      else
      {
        /* Wrong parameter specified, neither edition mode nor intial rect */
        sciprint(_("%s: Wrong type for input argument: Matrix or boolean expected.\n"), "fname");
      }
    }
  }
  else if (initialRectSpecified == -1)
  {
    /* error in initial rect */
    return 0;
  }

  /* Put values into the stack and return */
  return returnRectAndButton(selectedRect, button);
} 

/*--------------------------------------------------------------------------*/
