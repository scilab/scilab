/*------------------------------------------------------------------------*/
/* file: get_z_label_property.c                                           */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the z_label field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_z_label_property( sciPointObj * pobj )
{
  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property undefined for this object.\n"), "z_label") ;
    return -1 ;
  }
  return sciReturnHandle( sciGetHandle( pSUBWIN_FEATURE(pobj)->mon_z_label ) ) ;
}
/*------------------------------------------------------------------------*/
