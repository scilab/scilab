/*------------------------------------------------------------------------*/
/* file: get_grid_property.c                                              */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the grid field of                */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_grid_property( sciPointObj * pobj )
{
  double grid[3] ;

  if (sciGetEntityType (pobj) != SCI_SUBWIN) 
  {
    sciprint(_("%s property does not exist for this handle.\n"),"grid") ;
    return -1 ;
  }

  /* need converstion for display in double */
  grid[0] = pSUBWIN_FEATURE(pobj)->grid[0] ;
  grid[1] = pSUBWIN_FEATURE(pobj)->grid[1] ;
  grid[2] = pSUBWIN_FEATURE(pobj)->grid[2] ;

  if ( sciGetIs3d( pobj ) )
  {
    return sciReturnRowVector( grid, 3 ) ;
  }
  else
  {
    return sciReturnRowVector( grid, 2 ) ;
  }

}
/*------------------------------------------------------------------------*/
