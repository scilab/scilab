/*------------------------------------------------------------------------*/
/* file: set_font_foreground_property.c                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the font_angle field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "GetProperty.h"

/*------------------------------------------------------------------------*/
int set_font_foreground_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  return sciSetFontForeground( pobj, (int) getDoubleFromStack( stackPointer ) ) ;
}
/*------------------------------------------------------------------------*/
