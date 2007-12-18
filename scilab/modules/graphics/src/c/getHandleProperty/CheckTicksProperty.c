/*------------------------------------------------------------------------*/
/* file: CheckTicksProperty.h                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : generic function to retrieve the assigned x_ticks, y_ticks or   */
/*        z_ticks property                                                */
/*------------------------------------------------------------------------*/

#include "CheckTicksProperty.h"

/*----------------------------------------------------------------------------------*/
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
    sciprint("locations should be a vector of double.\n") ;
    return NULL ;
  }

  getCurrentDoubleMatrixFromList( tlist, &nbRowLoc, &nbColLoc ) ;

  if ( nbRowLoc * nbColLoc == 0 )
  {
    /* labels should also be an empty matrix */
    if ( !isListCurrentElementEmptyMatrix( tlist ) )
    {
      sciprint("Ticks location and label vectors must have the same size.\n") ;
      return NULL ;
    }  
  }
  else
  {
    if ( !isListCurrentElementStringMatrix( tlist ) )
    {
      sciprint("labels should be a string vector.\n") ;
      return NULL ;
    }

    getCurrentStringMatrixFromList( tlist, &nbRowLab, &nbColLab ) ;

    if ( nbRowLoc != nbRowLab || nbColLoc != nbColLab )
    {
      sciprint("Ticks location and label vectors must have the same size.\n") ;
      return NULL ;
    }
  }

  rewindAssignedList( tlist ) ;

  return tlist ;
  
}
/*----------------------------------------------------------------------------------*/
