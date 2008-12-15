/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/* file: CheckTicksProperty.h                                             */
/* desc : generic function to retrieve the assigned x_ticks, y_ticks or   */
/*        z_ticks property                                                */
/*------------------------------------------------------------------------*/

#include "CheckTicksProperty.h"
#include "localization.h"
#include "Scierror.h"
/*---------------------------------------------------------------------------*/

AssignedList * createTlistForTicks( void )
{
  AssignedList *  tlist     = NULL ;
  int             nbRowLoc  = 0    ;
  int             nbColLoc  = 0    ;
  int             nbRowLab  = 0    ;
  int             nbColLab  = 0    ;

  tlist = createAssignedList( 3, 2 ) ;

  if( !isListCurrentElementDoubleMatrix( tlist ) )
  {
    Scierror(999, _("%s should be a vector of double.\n"),"locations") ;
    return NULL ;
  }

  getCurrentDoubleMatrixFromList( tlist, &nbRowLoc, &nbColLoc ) ;

  if ( nbRowLoc * nbColLoc == 0 )
  {
    /* labels should also be an empty matrix */
    if ( !isListCurrentElementEmptyMatrix( tlist ) )
    {
      Scierror(999, _("Ticks location and label vectors must have the same size.\n")) ;
      return NULL ;
    }  
  }
  else
  {
    if ( !isListCurrentElementStringMatrix( tlist ) )
    {
      Scierror(999, _("%s should be a string vector.\n"),"labels") ;
      return NULL ;
    }

    getCurrentStringMatrixFromList( tlist, &nbRowLab, &nbColLab ) ;

    if ( nbRowLoc != nbRowLab || nbColLoc != nbColLab )
    {
      Scierror(999, _("Ticks location and label vectors must have the same size.\n"));
      return NULL ;
    }
  }

  rewindAssignedList( tlist ) ;

  return tlist ;
  
}
/*---------------------------------------------------------------------------*/
