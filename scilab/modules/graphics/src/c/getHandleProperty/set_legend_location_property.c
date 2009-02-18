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
/* file: set_x_location_property.c                                        */
/* desc : function to modify in Scilab the legend_location (place) field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_legend_location_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  
  if ( !isParameterStringMatrix( valueType ) )
  {
    Scierror(999, _("Incompatible type for property %s.\n"),"legend_location") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_LEGEND )
  {
    Scierror(999, _("%s property does not exist for this handle.\n"),"legend_location") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "in_upper_right" ) )
  {
    pLEGEND_FEATURE(pobj)->place = SCI_LEGEND_IN_UPPER_RIGHT;
  }
  else if ( isStringParamEqual( stackPointer, "in_upper_left" ) )
  {
    pLEGEND_FEATURE(pobj)->place = SCI_LEGEND_IN_UPPER_LEFT;
  }
 else if ( isStringParamEqual( stackPointer, "in_lower_right" ) )
  {
    pLEGEND_FEATURE(pobj)->place = SCI_LEGEND_IN_LOWER_RIGHT;
  }
  else if ( isStringParamEqual( stackPointer, "in_lower_left" ) )
  {
    pLEGEND_FEATURE(pobj)->place = SCI_LEGEND_IN_LOWER_LEFT;
  }

  else if ( isStringParamEqual( stackPointer, "out_upper_right" ) )
  {
    pLEGEND_FEATURE(pobj)->place = SCI_LEGEND_OUT_UPPER_RIGHT;
  }
  else if ( isStringParamEqual( stackPointer, "out_upper_left" ) )
  {
    pLEGEND_FEATURE(pobj)->place = SCI_LEGEND_OUT_UPPER_LEFT;
  }
 else if ( isStringParamEqual( stackPointer, "out_lower_right" ) )
  {
    pLEGEND_FEATURE(pobj)->place = SCI_LEGEND_OUT_LOWER_RIGHT;
  }
  else if ( isStringParamEqual( stackPointer, "out_lower_left" ) )
  {
    pLEGEND_FEATURE(pobj)->place = SCI_LEGEND_OUT_LOWER_LEFT;
  }
  else if ( isStringParamEqual( stackPointer, "upper_caption" ) )
  {
    pLEGEND_FEATURE(pobj)->place = SCI_LEGEND_UPPER_CAPTION;
  }
  else if ( isStringParamEqual( stackPointer, "lower_caption" ) )
  {
    pLEGEND_FEATURE(pobj)->place = SCI_LEGEND_LOWER_CAPTION;
  }
  else if ( isStringParamEqual( stackPointer, "by_coordinates" ) )
  {
    pLEGEND_FEATURE(pobj)->place = SCI_LEGEND_BY_COORDINATES;
  }

  else  
  {
    Scierror(999, _("%s: Wrong type for input argument #%d: '%s', '%s' or '%s' expected.\n"), "set_legend_location_property",2,"top","bottom","origin") ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
