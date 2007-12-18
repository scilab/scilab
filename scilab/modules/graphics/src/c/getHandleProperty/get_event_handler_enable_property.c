/*------------------------------------------------------------------------*/
/* file: get_event_handler_enable_property.c                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : function to retrieve in Scilab the event_handler_enable field   */
/*        of a handle                                                     */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_event_handler_enable_property( sciPointObj * pobj )
{

  if ( sciGetEntityType (pobj) != SCI_FIGURE )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"event_handler_enable") ;
    return -1 ;
  }

  if ( sciGetIsEventHandlerEnable( pobj ) )
  {
    return sciReturnString( "on" ) ;
  }
  return sciReturnString( "off" ) ;

}
/*------------------------------------------------------------------------*/
