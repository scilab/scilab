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
/* file: set_auto_clear_property.c                                        */
/* desc : function to modify in Scilab the auto_clear field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_auto_clear_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  if ( !isParameterStringMatrix( valueType ) )
  {
    Scierror(999, _("Incompatible type for property %s.\n"),"auto_clear") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "on" ) )
  {
    return sciSetAddPlot( pobj, FALSE ) ;
  }
  else if ( isStringParamEqual( stackPointer, "off" ) )
  {
    return sciSetAddPlot( pobj, TRUE ) ;
  }
  else
  {
    Scierror(999, _("%s: Wrong input argument: '%s' or '%s' expected.\n"),"set_auto_clear_property","on","off");
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/

