/*------------------------------------------------------------------------*/
/* file: set_font_size_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the polyline_style field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"

/*------------------------------------------------------------------------*/
int set_font_size_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  return sciSetFontDeciWidth( pobj, ((int)getDoubleFromStack( stackPointer )) );
}
/*------------------------------------------------------------------------*/
