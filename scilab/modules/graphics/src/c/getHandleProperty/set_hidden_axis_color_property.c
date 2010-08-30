/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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
#include "SetPropertyStatus.h"

// sciGetNumColors prototype
#include "GetProperty.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_hidden_axis_color_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  BOOL status;
  int haColor      = (int) getDoubleFromStack( stackPointer );

  /*
   * To be implemented using the MVC framework
   * since it uses the parent/child relationship in
   * order to get the parent figure's number of colors
   */
  int colormapSize = sciGetNumColors( pobj );

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Integer expected.\n"), "hidden_axis_color");
    return SET_PROPERTY_ERROR ;
  }

#if 0
  if ( sciGetEntityType (pobj) != SCI_SUBWIN )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"hidden_axis_color");
    return SET_PROPERTY_ERROR;
  }
#endif

  if ( haColor >= -2 && haColor <= colormapSize + 1 )
  {
    status = setGraphicObjectProperty(pobj->UID, __GO_HIDDEN_AXIS_COLOR__, &haColor, jni_int, 1);

    /* To be implemented using the MVC framework, since it performs color range checks */
//    return sciSetHiddenAxisColor(pobj, haColor);

    if (status == TRUE)
    {
      return SET_PROPERTY_SUCCEED;
    }
    else
    {
      Scierror(999, _("'%s' property does not exist for this handle.\n"),"hidden_axis_color");
      return SET_PROPERTY_ERROR;
    }

  }
  else
  {
    Scierror(999, _("Wrong value for '%s' property: Must be a valid color index.\n"), "hidden_axis_color");
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
