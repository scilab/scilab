/*------------------------------------------------------------------------*/
/* file: get_view_property.c                                              */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the view field of                */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_view_property( sciPointObj * pobj )
{

  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"view") ;
    return -1;
  }

  if ( sciGetIs3d( pobj ) )
  {
    return sciReturnString( "3d" ) ;
  }
  else
  {
    return sciReturnString( "2d" ) ;
  }
}
/*------------------------------------------------------------------------*/
