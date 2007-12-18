/*------------------------------------------------------------------------*/
/* file: get_surface_mode_property.c                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the surface_mode field of a      */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_surface_mode_property( sciPointObj * pobj )
{
  if ( (sciGetEntityType(pobj) == SCI_PLOT3D ) ||
       (sciGetEntityType(pobj) == SCI_FAC3D  ) ||
       (sciGetEntityType(pobj) == SCI_SURFACE)   )
  {
    if ( sciGetIsLine( pobj ) )
    {
      return sciReturnString( "on" ) ;
    }
    else
    {
      return sciReturnString( "off" ) ;
    }
  }
  else
  {
    sciprint("Surface_mode value can not be accessed with this object, use line_mode.\n") ;
    return -1;
  }
}
/*------------------------------------------------------------------------*/
