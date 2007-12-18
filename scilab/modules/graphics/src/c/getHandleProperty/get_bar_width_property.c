/*------------------------------------------------------------------------*/
/* file: get_bar_width_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the bar_width field of a         */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_bar_width_property( sciPointObj * pobj )
{
  if ( sciGetEntityType (pobj) == SCI_POLYLINE )
  {
    return sciReturnDouble( pPOLYLINE_FEATURE (pobj)->bar_width ) ;
  }
  else
  { 
    sciprint(_("%s property does not exist for this handle.\n"),"bar_with");
    return -1 ;
  }
}
/*------------------------------------------------------------------------*/
