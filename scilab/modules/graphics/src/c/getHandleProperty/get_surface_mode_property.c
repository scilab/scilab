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
/* file: get_surface_mode_property.c                                      */
/* desc : function to retrieve in Scilab the surface_mode field of a      */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_surface_mode_property( sciPointObj * pobj )
{
  if ( (sciGetEntityType(pobj) == SCI_PLOT3D ) ||
       (sciGetEntityType(pobj) == SCI_FAC3D  ) ||
       (sciGetEntityType(pobj) == SCI_SURFACE)   )
  {
    if ( sciGetIsLine( pobj ) )
    {
      return sciReturnString( "on" ) ;
    }
    else
    {
      return sciReturnString( "off" ) ;
    }
  }
  else
  {
		Scierror(999, _("'%s' property does not exist for this handle.\n"),"surface_mode") ;
    return -1;
  }
}
/*------------------------------------------------------------------------*/
