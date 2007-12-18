/*------------------------------------------------------------------------*/
/* file: get_pixmap_property.c                                            */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the pixmap field of a            */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_pixmap_property( sciPointObj * pobj )
{
  if ( sciGetEntityType(pobj) != SCI_FIGURE )
  {
    sciprint("pixmap property undefined for this object") ;
    return -1;
  }
  if ( sciGetPixmapMode(pobj) )
  {
    return sciReturnString( "on" ) ;
  }
  else
  {
    return sciReturnString( "off" ) ;
  }
}
/*------------------------------------------------------------------------*/
