/*------------------------------------------------------------------------*/
/* file: get_visible_property.c                                           */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the visible field of a handle    */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "getHandleProperty.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "returnProperty.h"

/*------------------------------------------------------------------------*/

int get_visible_property( sciPointObj * pobj )
{

  if ( sciGetVisibility( pobj ) )
  {
    return sciReturnString( "on" ) ;
  }
  else
  {
    return sciReturnString( "off" ) ;
  }

  return 0 ;
}

/*------------------------------------------------------------------------*/
