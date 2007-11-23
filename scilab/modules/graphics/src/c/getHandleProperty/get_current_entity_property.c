/*------------------------------------------------------------------------*/
/* file: get_current_entity_property.c                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the current_entity field of a    */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "CurrentObjectsManagement.h"

/*--------------------------------------------------------------------------*/
int get_current_entity_property( sciPointObj * pobj )
{
  return sciReturnHandle( sciGetHandle(sciGetCurrentObj()) ) ;
}
/*--------------------------------------------------------------------------*/
