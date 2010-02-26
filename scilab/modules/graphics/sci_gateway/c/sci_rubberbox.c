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
#include "gw_graphics.h"
#include "stack-c.h"
#include "localization.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "Interaction.h"
#include "CurrentObjectsManagement.h"

/*--------------------------------------------------------------------------*/
static int getInitialRectangle(double initRect[4]);
static int getEditionMode(int rhsPos);
static int returnRectAndButton(const double selectedRect[4], int button);
/*--------------------------------------------------------------------------*/
static int getInitialRectangle(double initRect[4])
{
  int rectNbRow = 0;
  int rectNbCol = 0;
  int rectStackPointer = 0;
	int i;
	int nbDims;
	double * rect;

	/* Initial rect is always in first position */
  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&rectNbRow,&rectNbCol,&rectStackPointer);

	/* We have found a matrix of double within the parameters check its dims */
  nbDims = rectNbCol * rectNbRow;
  if (nbDims != 2 && nbDims != 4)
  {
    return -1;
  }
    

  /* pointer on the stack */
  rect = getDoubleMatrixFromStack(rectStackPointer);

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
	return 1;
}
/*--------------------------------------------------------------------------*/
static int getEditionMode(int rhsPos)
{
	int stackPointer = 0;
	int nbRow = 0;
	int nbCol = 0;
	BOOL isEditionModeOn;
	GetRhsVar(rhsPos,MATRIX_OF_BOOLEAN_DATATYPE,&nbRow,&nbCol,&stackPointer);

	if (nbRow * nbCol != 1)
	{
    return -1;
	}

	isEditionModeOn = *istk(stackPointer);
	if (isEditionModeOn)
	{
		return 1;
	}
	else
	{
		return 0;
	}

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

  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/
int sci_rubberbox(char * fname, unsigned long fname_len)
{
  
  /* [final_rect, btn] = rubberbox([initial_rect],[edition_mode]) */
  
  int button = 0;
  double selectedRect[4];

  CheckRhs(0,2);
  CheckLhs(1,2);

	if (Rhs == 0)
	{
		/* rubberbox() */
		rubberBox(sciGetCurrentSubWin(), TRUE, NULL, selectedRect, &button);
	}
	else if (Rhs == 1)
	{
		/* rubberbox(initial_rect) or rubberbox(edition_mode) */
		if (GetType(1) == sci_matrix)
		{
			/* rubberbox(initial_rect) */
			/* Default values, intial rect and edition mode to false */
			double initialRect[4] = {0.0, 0.0, 0.0, 0.0};
			
			if (getInitialRectangle(initialRect) == 1)
			{
				rubberBox(sciGetCurrentSubWin(), TRUE, initialRect, selectedRect, &button);
			}
			else
			{
				Scierror(999, _("%s: Wrong size for input argument #%d: Vector of size %d or %d expected.\n"), fname, 1, 2, 4);
				return -1;
			}
		}
		else if (GetType(1) == sci_boolean)
		{
			/* rubberbox(editionMode) */
			int editionModeStatus = getEditionMode(1);
			if (editionModeStatus == 1)
			{
				/* rubberbox(%t) */
				rubberBox(sciGetCurrentSubWin(), FALSE, NULL, selectedRect, &button);
			}
			else if (editionModeStatus == 0)
			{
				/* rubberbox(%f) */
				rubberBox(sciGetCurrentSubWin(), TRUE, NULL, selectedRect, &button);
			}
			else
			{
				Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected\n."), fname, 1);
				return -1;
			}
		}
		else
		{
			/* Wrong parameter specified, neither edition mode nor intial rect */
			Scierror(999, _("%s: Wrong type for input argument #%d: Real row vector or a boolean expected.\n"), fname, 1);
			return -1;
		}
	}
	else if (Rhs == 2)
	{
		/* rubberbox(initial_rect, edition_mode) */

		/* Default values, intial rect and edition mode to false */
		double initialRect[4] = {0.0, 0.0, 0.0, 0.0};
		int editionModeStatus;
		
		if (GetType(1) != sci_matrix)
		{
			Scierror(999, _("%s: Wrong type for input argument #%d: Real row vector expected.\n"), fname, 1);
			return -1;
		}

		if (GetType(2) != sci_boolean)
		{
			Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 2);
			return -1;
		}

		/* Getting initial rect */
		if(getInitialRectangle(initialRect) != 1)
		{
			Scierror(999, _("%s: Wrong size for input argument #%d: Vector of size %d or %d expected.\n"), fname, 1, 2, 4);
			return -1;
		}

		/* Getting edition mode */
		editionModeStatus = getEditionMode(2);

		if (editionModeStatus == 1)
		{
			/* rubberbox(initial_rect, %t) */
			rubberBox(sciGetCurrentSubWin(), FALSE, initialRect, selectedRect, &button);
		}
		else if (editionModeStatus == 0)
		{
			/* rubberbox(initial_rect, %f) */
			rubberBox(sciGetCurrentSubWin(), TRUE, initialRect, selectedRect, &button);
		}
		else
		{
			Scierror(999, _("%s: Wrong size for input argument #%d: A boolean expected\n."), fname, 2);
			return -1;
		}
		
	}


  /* Put values into the stack and return */
  return returnRectAndButton(selectedRect, button);
} 

/*--------------------------------------------------------------------------*/
