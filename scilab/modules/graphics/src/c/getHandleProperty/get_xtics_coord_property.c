/*------------------------------------------------------------------------*/
/* file: get_xtics_coord_property.c                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the xtics_coord field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_xtics_coord_property( sciPointObj * pobj )
{
  if ( sciGetEntityType(pobj) != SCI_AXES )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"xtics_coord") ;
    return -1 ;
  }
  return sciReturnRowVector( pAXES_FEATURE(pobj)->vx, pAXES_FEATURE(pobj)->nx ) ;
}
/*------------------------------------------------------------------------*/
