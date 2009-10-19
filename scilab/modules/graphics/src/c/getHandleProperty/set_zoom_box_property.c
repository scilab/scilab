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
/* file: set_zoom_box_property.c                                          */
/* desc : function to modify in Scilab the zoom_box field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "axesScale.h"

/*------------------------------------------------------------------------*/
int set_zoom_box_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "zoom_box");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"zoom_box") ;
    return SET_PROPERTY_ERROR ;
  }

  /* We must have a 4x1 matrix */
  if ( nbRow * nbCol == 6 )
  {
    return sciZoom3D(pobj, getDoubleMatrixFromStack(stackPointer));
  }
  else if( nbRow * nbCol == 4)
  {
    return sciZoom2D(pobj, getDoubleMatrixFromStack(stackPointer));
  }
  else if ( nbCol * nbRow == 0 )
  {
    sciUnzoomSubwin(pobj);
  }
  else
  {
    Scierror(999, _("Wrong size for '%s' property: Must be in the set {%s}.\n"), "zoom_box", "0x0, 2x2, 2x3");
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
