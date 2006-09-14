/*------------------------------------------------------------------------*/
/* file: get_parent_property.c                                            */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the parent field of a            */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"

/*------------------------------------------------------------------------*/
int get_parent_property( sciPointObj * pobj )
{
  return sciReturnHandle( sciGetHandle( sciGetParent( pobj ) ) ) ;
}
/*------------------------------------------------------------------------*/
