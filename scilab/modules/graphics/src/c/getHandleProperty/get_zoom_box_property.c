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
/* file: get_zoom_box_property.c                                          */
/* desc : function to retrieve in Scilab the zoom_box field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "axesScale.h"

/*------------------------------------------------------------------------*/
int get_zoom_box_property( sciPointObj * pobj )
{
  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"zoom_box") ;
    return -1 ;
  }

  if ( sciGetZooming( pobj ) )
  {
    double zoomBox[6];
    sciGetZoom3D(pobj, zoomBox);
    return sciReturnRowVector( zoomBox, 6 ) ;
  }
  else
  {
    return sciReturnEmptyMatrix() ;
  }
}
/*------------------------------------------------------------------------*/
