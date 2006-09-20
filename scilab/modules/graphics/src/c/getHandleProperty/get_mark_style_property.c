/*------------------------------------------------------------------------*/
/* file: get_mark_style_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the mark_style field of a        */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"

/*------------------------------------------------------------------------*/
int get_mark_style_property( sciPointObj * pobj )
{
  return sciReturnDouble( sciGetMarkStyle( pobj ) ) ;
}
/*------------------------------------------------------------------------*/
