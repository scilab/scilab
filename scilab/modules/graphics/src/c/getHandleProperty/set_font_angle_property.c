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
/* file: set_font_angle_property.c                                        */
/* desc : function to modify in Scilab the font_angle field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "math_graphics.h"

/*------------------------------------------------------------------------*/
int set_font_angle_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real expected.\n"), "font_angle");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetAutoRotation( pobj ) )
  {
    sciSetAutoRotation( pobj, FALSE ) ;
  }
  return sciSetFontOrientation( pobj, DEG2RAD(getDoubleFromStack(stackPointer))  ) ;

}
/*------------------------------------------------------------------------*/
