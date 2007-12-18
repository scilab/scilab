/*------------------------------------------------------------------------*/
/* file: get_margins_property.c                                           */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the event_handler field of       */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_event_handler_property( sciPointObj * pobj )
{

  if ( sciGetEntityType(pobj) != SCI_FIGURE )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"event_handler") ;
    return -1 ;
  }

  return sciReturnString( sciGetEventHandler( pobj ) ) ;

}
/*------------------------------------------------------------------------*/
