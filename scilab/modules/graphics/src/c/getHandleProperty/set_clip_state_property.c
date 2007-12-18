/*------------------------------------------------------------------------*/
/* file: set_clip_state_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the clip_state field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_clip_state_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"clip_state") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"axes_visible") ;
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
    sciprint("Value must be 'clipgrf', 'on' or 'off'.\n") ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
