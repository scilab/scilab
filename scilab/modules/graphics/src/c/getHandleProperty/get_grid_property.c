/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
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
/* file: get_grid_property.c                                              */
/* desc : function to retrieve in Scilab the grid field of                */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_grid_property( sciPointObj * pobj )
{
  double grid[3] ;

  if (sciGetEntityType (pobj) != SCI_SUBWIN) 
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"grid") ;
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
