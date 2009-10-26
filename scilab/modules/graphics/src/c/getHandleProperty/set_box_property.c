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
/* file: set_box_property.c                                               */
/* desc : function to modify in Scilab the box field of                   */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_box_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "box");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType( pobj ) == SCI_SUBWIN )
  {
    if ( isStringParamEqual( stackPointer, "off" ) )
    {
      return sciSetBoxType( pobj, BT_OFF ) ;
    }
    else if ( isStringParamEqual( stackPointer, "on" ) )
    {
      return sciSetBoxType( pobj, BT_ON ) ;
    }
    else if ( isStringParamEqual( stackPointer, "hidden_axes" ) )
    {
      return sciSetBoxType( pobj, BT_HIDDEN_AXES ) ;
    }
    else if ( isStringParamEqual( stackPointer, "hidden_axis" ) )
    {
      sciprint(_("WARNING !!!\nIn '%s' property: '%s' is deprecated use '%s' instead.\n"), "box", "hidden_axis", "hidden_axes");
      return sciSetBoxType( pobj, BT_HIDDEN_AXES ) ;
    }
    else if ( isStringParamEqual( stackPointer, "back_half" ) )
    {
      return sciSetBoxType( pobj, BT_BACK_HALF ) ;
    }
    else
    {
      Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "box", "on, off, hidden_axes, back_half");
      return SET_PROPERTY_ERROR ;
    }
  }
  else if (sciGetEntityType( pobj ) == SCI_TEXT)
  {
    if ( isStringParamEqual( stackPointer, "on" ) )
    {
      return sciSetIsBoxed( pobj, TRUE ) ;
    }
    else if ( isStringParamEqual( stackPointer, "off" ) )
    {
      return sciSetIsBoxed( pobj, FALSE ) ;
    }
    else
    {
      Scierror(999, _("Wrong value for '%s' property: %s or %s expected.\n"), "box", "on", "off");
      return SET_PROPERTY_SUCCEED ;
    }
  }
	

	Scierror(999, _("'%s' property does not exist for this handle.\n"),"box") ;
  return SET_PROPERTY_ERROR ;

}
/*------------------------------------------------------------------------*/
