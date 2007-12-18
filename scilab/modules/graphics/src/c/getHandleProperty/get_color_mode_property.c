/*------------------------------------------------------------------------*/
/* file: get_color_mode_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the color_mode field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_color_mode_property( sciPointObj * pobj )
{
  if ( sciGetEntityType (pobj) != SCI_SURFACE )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"color_mode") ;
    return -1;
  }
  return sciReturnDouble( pSURFACE_FEATURE (pobj)->flag[0] ) ;
}
/*------------------------------------------------------------------------*/
