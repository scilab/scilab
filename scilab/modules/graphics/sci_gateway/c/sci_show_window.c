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
/* file: sci_show_window.c                                                */
/* desc : interface for show_window routine                               */
/*------------------------------------------------------------------------*/

#include "sci_show_window.h"
#include "getHandleProperty/getPropertyAssignedValue.h"
#include "HandleManagement.h"
#include "stack-c.h"
#include "sciprint.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "InitObjects.h"
#include "CurrentObjectsManagement.h"
#include "localization.h"
#include "Interaction.h"
#include "WindowList.h"
/*--------------------------------------------------------------------------*/
int sci_show_window( char *fname,unsigned long fname_len )
{
  sciPointObj * shownFigure = NULL;

  CheckRhs(0,1);
  CheckLhs(0,1);

  if ( Rhs == 1 )
  {
    /* the window to show is specified */
    int paramType    = VarType(1);
    int nbRow        = 0 ;
    int nbCol        = 0 ;
    size_t stackPointer = 0 ;

    if ( isParameterHandle( paramType ) )
    {
      /* by tis handle */
      GetRhsVar( 1,GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stackPointer );

      if ( nbRow * nbCol != 1 )
      {
        sciprint(_("%s: Wrong size for input argument #%d: A '%s' handle or a real scalar expected.\n"),fname, 1, "Figure");
        return -1 ;
      }

      shownFigure = sciGetPointerFromHandle( getHandleFromStack(stackPointer) );

      if (sciGetEntityType(shownFigure) != SCI_FIGURE)
      {
        sciprint(_("%s: Wrong type for input argument #%d: A '%s' handle or a real scalar expected.\n"),fname, 1, "Figure");
        return -1 ;
      }

    }
    else if ( isParameterDoubleMatrix( paramType ) )
    {
      /* by its number */
      int winNum;
      GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &stackPointer );
      if ( nbRow * nbCol != 1 )
      {
        sciprint(_("%s: Wrong size for input argument #%d: A '%s' handle or a real scalar expected.\n"),fname, 1, "Figure");
        return -1 ;
      }
      winNum = (int) getDoubleFromStack(stackPointer);
      shownFigure = getFigureFromIndex(winNum);

      if (shownFigure == NULL)
      {
        /* No window with this number, create one */
        if(sciSetUsedWindow(winNum) < 0)
        {
          sciprint(_("%s: Unable to create requested figure: No more memory.\n"), fname);
        }
        shownFigure = sciGetCurrentFigure();
      }
    }
    else
    {
      sciprint(_("%s: Wrong type for input argument #%d: A '%s' handle or a real scalar expected.\n"),fname,1, "Figure");
    }

  }
  else
  {
    /* Rhs == 0 */
    /* raise current figure */
    shownFigure = sciGetCurrentFigure();
  }

  /* Check that the requested figure really exists */
  if ( shownFigure == NULL )
  {
    sciprint(_("%s: '%s' handle does not or no longer exists.\n"),fname,"Figure");
    return -1 ;
  }

  /* Actually show the window */
  showWindow(shownFigure);

  LhsVar(1)=0;
  return 0;
}
/*--------------------------------------------------------------------------*/
