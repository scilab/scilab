/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_clip_state_property.c                                        */
/* desc : function to modify in Scilab the clip_state field of            */
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
int set_clip_state_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "clip_state");
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "clipgrf" ) )
  {
    return sciSetIsClipping( pobj, 0 ) ;
  }
  else if ( isStringParamEqual( stackPointer, "off" ) )
  {
    return sciSetIsClipping(  pobj, -1 ) ;
  }
  else if ( isStringParamEqual( stackPointer, "on" ) )
  {
    if ( sciGetIsClipRegionValuated(pobj) == 1 )
    {
      return sciSetIsClipping( pobj, 1 ) ;
    }
    else
    {
      return sciSetIsClipping( pobj, 0 ) ;
    }
  }
  else
  {
    Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "clip_state", "on, off, clipgrf");
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
