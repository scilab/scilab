/*------------------------------------------------------------------------*/
/* file: get_mark_size_unit_property.c                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the mark_size_unit field of a    */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"

/*------------------------------------------------------------------------*/
int get_mark_size_unit_property( sciPointObj * pobj )
{
  if (sciGetMarkSizeUnit( pobj ) == 1)
  {
    return sciReturnString( "point" ) ;
  }
  else if( sciGetMarkSizeUnit(pobj) == 2 )
  {
    return sciReturnString( "tabulated" ) ;
  }
  return -1 ;
}
/*------------------------------------------------------------------------*/
