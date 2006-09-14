/*------------------------------------------------------------------------*/
/* file: get_type_property.c                                              */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the type field of a              */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"

/*------------------------------------------------------------------------*/
int get_type_property( sciPointObj * pobj )
{
  return sciReturnString( sciGetCharEntityType( pobj ) ) ;
}
/*------------------------------------------------------------------------*/
