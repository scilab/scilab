/*------------------------------------------------------------------------*/
/* file: set_color_map_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the color_map field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"

/*------------------------------------------------------------------------*/
int set_color_map_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  return sciSetColormap( pobj, getDoubleMatrixFromStack( stackPointer), nbRow, nbCol ) ;
}
/*------------------------------------------------------------------------*/
