/*------------------------------------------------------------------------*/
/* file: get_auto_scale_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the auto_scale field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"

/*------------------------------------------------------------------------*/
int get_auto_scale_property( sciPointObj * pobj )
{
  if ( sciGetAutoScale( pobj ) )
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
