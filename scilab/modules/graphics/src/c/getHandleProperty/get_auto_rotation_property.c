/*------------------------------------------------------------------------*/
/* file: get_auto_rotation_property.c                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the auto_rotation field of       */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_auto_rotation_property( sciPointObj * pobj )
{
  if( sciGetEntityType(pobj) != SCI_LABEL )
  {
    sciprint("auto_rotation does not exist for this handle.\n") ;
    return -1 ;
  }

  if ( sciGetAutoRotation(pobj) )
  {
    return sciReturnString( "on" ) ;
  }
  else
  {
    return sciReturnString( "off" ) ;
  }
}
/*------------------------------------------------------------------------*/
