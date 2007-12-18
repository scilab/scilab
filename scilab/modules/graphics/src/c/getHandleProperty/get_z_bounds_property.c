/*------------------------------------------------------------------------*/
/* file: get_z_bounds_property.c                                          */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the z_bounds field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_z_bounds_property( sciPointObj * pobj )
{
  if (sciGetEntityType (pobj) != SCI_FEC)
  {
    sciprint(_("%s property does not exist for this handle.\n"),"z_bounds") ;
    return -1 ;
  }
  
  return sciReturnRowVector( pFEC_FEATURE(pobj)->zminmax, 2 ) ;

}
/*------------------------------------------------------------------------*/
