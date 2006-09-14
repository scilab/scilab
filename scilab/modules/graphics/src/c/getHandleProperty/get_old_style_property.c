/*------------------------------------------------------------------------*/
/* file: get_old_style_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the old_style field of a handle  */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "returnProperty.h"

/*------------------------------------------------------------------------*/

int get_old_style_property( sciPointObj * pobj )
{

  if ( version_flag() != 0)
  {
    return sciReturnString( "on" ) ;
  }
  else {
    return sciReturnString( "off" ) ;
  }

  return 0 ;
}

/*------------------------------------------------------------------------*/
