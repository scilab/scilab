/*------------------------------------------------------------------------*/
/* file: set_event_handler_enable_property.c                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : function to modify in Scilab the event_handler_enable field of  */
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
int set_event_handler_enable_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"event_handler") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType( pobj ) != SCI_FIGURE )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"event_handler");
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "on" ) )
  {
    return sciSetIsEventHandlerEnable( pobj, TRUE ) ;
  }
  else if ( isStringParamEqual( stackPointer, "off" ) )
  {
    return sciSetIsEventHandlerEnable( pobj, FALSE ) ;
  }
  sciprint(_("%s: Wrong type for second input argument: '%s' or '%s' expected.\n"), "set_event_handler_enable_property","on","off");
  return SET_PROPERTY_ERROR ;

}
/*------------------------------------------------------------------------*/
