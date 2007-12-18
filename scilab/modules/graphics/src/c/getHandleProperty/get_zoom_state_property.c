/*------------------------------------------------------------------------*/
/* file: get_zoom_state_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the zoom_state field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_zoom_state_property( sciPointObj * pobj )
{
  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"zoom_state") ;
    return -1 ;
  }

  if ( sciGetZooming( pobj ) )
  {
    return sciReturnString( "on" ) ;
  }
  else {
    return sciReturnString( "off" ) ;
  }
}
/*------------------------------------------------------------------------*/
