/*------------------------------------------------------------------------*/
/* file: get_auto_position_property.c                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the auto_position field of       */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_auto_position_property( sciPointObj * pobj )
{
  if( sciGetEntityType(pobj) != SCI_LABEL )
  {
    sciprint(_("%s does not exist for this handle.\n"), "auto_position") ;
    return -1 ;
  }

  if ( sciGetAutoPosition(pobj) )
  {
    return sciReturnString( "on" ) ;
  }
  else
  {
    return sciReturnString( "off" ) ;
  }
}
/*------------------------------------------------------------------------*/
