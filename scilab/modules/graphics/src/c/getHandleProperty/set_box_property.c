/*------------------------------------------------------------------------*/
/* file: set_box_property.c                                               */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
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
int set_box_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
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
      sciprint("Second argument must be 'on', 'off', 'hidden_axis' or 'back_half'.\n") ;
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
