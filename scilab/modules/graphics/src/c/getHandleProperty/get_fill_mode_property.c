/*------------------------------------------------------------------------*/
/* file: get_fill_mode_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the fill_mode field of a         */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"

/*------------------------------------------------------------------------*/
int get_fill_mode_property( sciPointObj * pobj )
{
  if ( sciGetIsFilled( pobj ) )
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
