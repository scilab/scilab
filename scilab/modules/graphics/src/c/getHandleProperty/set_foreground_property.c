/*------------------------------------------------------------------------*/
/* file: set_foreground_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the interp_color_mode field of     */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"

/*------------------------------------------------------------------------*/
int set_foreground_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  return sciSetForeground( pobj, (int) getDoubleFromStack( stackPointer ) ) ;
}
/*------------------------------------------------------------------------*/
