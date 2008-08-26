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
/* file: set_box_property.c                                               */
/* desc : function to modify in Scilab the box field of                   */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_box_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"box") ;
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
    else if ( isStringParamEqual( stackPointer, "hidden_axis" ) )
    {
      return sciSetBoxType( pobj, BT_HIDDEN_AXIS ) ;
    }
    else if ( isStringParamEqual( stackPointer, "back_half" ) )
    {
      return sciSetBoxType( pobj, BT_BACK_HALF ) ;
    }
    else
    {
      sciprint(_("%s: Wrong type for second input argument: '%s', '%s', '%s' or '%s' expected.\n"), "set_box_property","on","off","hidden_axis","back_half") ;
      return SET_PROPERTY_ERROR ;
    }
  }
  else
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
      sciprint(_("%s: Wrong second input argument: '%s' or '%s' expected."),"set_box_property","on","off") ;
      return SET_PROPERTY_SUCCEED ;
    }
  }

  return SET_PROPERTY_ERROR ;

}
/*------------------------------------------------------------------------*/
