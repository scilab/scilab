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
    sciprint("event_handler property does not exist for this handle.\n");
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
  sciprint("Second argument must be 'on' or 'off'.\n");
  return SET_PROPERTY_ERROR ;

}
/*------------------------------------------------------------------------*/
