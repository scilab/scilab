/*------------------------------------------------------------------------*/
/* file: get_polyline_style_property.c                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the polyline_style field of a    */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_polyline_style_property( sciPointObj * pobj )
{
  if ( sciGetEntityType (pobj) == SCI_POLYLINE )
  {
    return sciReturnDouble( pPOLYLINE_FEATURE (pobj)->plot ) ;
  }
  else
  {
    sciprint("polyline_style does not exist for this handle.\n") ;
    return -1 ;
  }
}
/*------------------------------------------------------------------------*/
