/*------------------------------------------------------------------------*/
/* file: get_clip_state_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the clip_state field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"

/*------------------------------------------------------------------------*/
int get_clip_state_property( sciPointObj * pobj )
{
  if ( sciGetIsClipping (pobj) == 0 )
  {
    return sciReturnString( "clipgrf" ) ;
  }
  else if ( sciGetIsClipping (pobj) > 0 )
  {
    return sciReturnString( "on" ) ;
  }
  else
  {   
    return sciReturnString( "off" ) ;
  }
  return -1 ;
}
/*------------------------------------------------------------------------*/
