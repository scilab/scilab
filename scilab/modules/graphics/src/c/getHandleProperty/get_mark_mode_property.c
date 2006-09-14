/*------------------------------------------------------------------------*/
/* file: get_mark_mode_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the mark_mode field of a         */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"

/*------------------------------------------------------------------------*/
int get_mark_mode_property( sciPointObj * pobj )
{
  if ( sciGetIsMark( pobj ) )
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
