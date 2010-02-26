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
/* file: set_hidden_axis_color_property.c                                 */
/* desc : function to modify in Scilab the hidden_axis_color field of     */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_hidden_axis_color_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  int haColor      = (int) getDoubleFromStack( stackPointer ) ;
  int colormapSize = sciGetNumColors( pobj ) ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Integer expected.\n"), "hidden_axis_color");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType (pobj) != SCI_SUBWIN )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"hidden_axis_color") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( haColor >= -2 && haColor <= colormapSize + 1 )
  {
    return sciSetHiddenAxisColor(pobj, haColor);
  }
  else
  {
    Scierror(999, _("Wrong value for '%s' property: Must be a valid color index.\n"), "hidden_axis_color");
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
