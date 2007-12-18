/*------------------------------------------------------------------------*/
/* file: set_visible_property.c                                           */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the visible field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_visible_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  getStringFromStack( stackPointer ) ;

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"visible") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "on" ) )
  {
    return sciSetVisibility( pobj, TRUE ) ;
  }
  else if ( isStringParamEqual( stackPointer, "off" ) )
  {
    return sciSetVisibility( pobj, FALSE ) ;
  }
  else
  {
    sciprint("Value must be 'on' or 'off'.\n") ;
    return SET_PROPERTY_ERROR ;
  }
}
/*------------------------------------------------------------------------*/
