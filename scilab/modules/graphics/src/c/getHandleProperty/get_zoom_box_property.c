/*------------------------------------------------------------------------*/
/* file: get_zoom_box_property.c                                          */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the zoom_box field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_zoom_box_property( sciPointObj * pobj )
{
  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"zoom_box") ;
    return -1 ;
  }

  if ( sciGetZooming( pobj ) )
  {
    return sciReturnRowVector( pSUBWIN_FEATURE(pobj)->ZRect, 4 ) ;
  }
  else
  {
    return sciReturnEmptyMatrix() ;
  }
}
/*------------------------------------------------------------------------*/
