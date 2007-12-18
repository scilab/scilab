/*------------------------------------------------------------------------*/
/* file: get_ytics_coord_property.c                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the ytics_coord field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_ytics_coord_property( sciPointObj * pobj )
{
  if ( sciGetEntityType(pobj) != SCI_AXES )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"ytics_coord") ;
    return -1 ;
  }
  return sciReturnRowVector( pAXES_FEATURE(pobj)->vy, pAXES_FEATURE(pobj)->ny ) ;
}
/*------------------------------------------------------------------------*/
