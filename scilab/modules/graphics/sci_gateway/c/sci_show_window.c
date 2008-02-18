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
/*--------------------------------------------------------------------------*/
int sci_show_window( char *fname,unsigned long fname_len )
{
  int winNum = 0 ;
  sciPointObj * curFigure = sciGetCurrentFigure();

  CheckRhs(0,1);
  CheckLhs(0,1);

  if ( Rhs == 1 )
  {
    /* the window to show is specified */
    int paramType    = VarType(1);
    int nbRow        = 0 ;
    int nbCol        = 0 ;
    int stackPointer = 0 ;

    if ( isParameterHandle( paramType ) )
    {
      sciPointObj * shownFigure   = NULL ;

      GetRhsVar( 1,GRAPHICAL_HANDLE_DATATYPE, &nbRow, &nbCol, &stackPointer );

      if ( nbRow * nbCol != 1 )
      {
        sciprint(_("%s: Only one window can be shown.\n"),fname);
        return -1 ;
      }

      shownFigure = sciGetPointerFromHandle( getHandleFromStack(stackPointer) );

      if ( shownFigure == NULL )
      {
        sciprint(_("%s: Figure does not or no longer exists.\n"),fname);
        return -1 ;
      }

      winNum = sciGetNum( shownFigure );

    }
    else if ( isParameterDoubleMatrix( paramType ) )
    {
      GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &stackPointer );
      if ( nbRow * nbCol != 1 )
      {
        sciprint(_("Only one window can be shown.\n"));
        return -1 ;
      }
      winNum = (int) getDoubleFromStack(stackPointer);

    }
    else
    {
      sciprint(_("Parameter should be a handle on the window to redraw or it's number.\n"));
    }

    sciSetUsedWindow( winNum );

  }
  
  /* if no window were opened (ie curFigure is NULL) then we created the first figure */
  /* and it is already the selected one */
  if( Rhs == 1 && curFigure != NULL )
  {
    sciSetUsedWindow( sciGetNum( curFigure ) );
  }

  LhsVar(1)=0;
  return 0;
}
/*--------------------------------------------------------------------------*/
