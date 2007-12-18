/*------------------------------------------------------------------------*/
/* file: get_axes_bounds_property.c                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the axes_bounds field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_axes_bounds_property( sciPointObj * pobj )
{

  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"axes_bounds") ;
    return -1 ;
  }

  return sciReturnRowVector( pSUBWIN_FEATURE (pobj)->WRect, 4 ) ;
}
/*------------------------------------------------------------------------*/
