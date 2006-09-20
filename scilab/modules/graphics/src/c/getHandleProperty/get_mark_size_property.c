/*------------------------------------------------------------------------*/
/* file: get_mark_size_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the mark_size field of a         */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"

/*------------------------------------------------------------------------*/
int get_mark_size_property( sciPointObj * pobj )
{
  return sciReturnDouble( sciGetMarkSize( pobj ) ) ;
}
/*------------------------------------------------------------------------*/
