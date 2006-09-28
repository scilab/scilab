/*------------------------------------------------------------------------*/
/* file: set_line_style_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the line_style field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_line_style_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  return sciSetLineStyle( pobj, (int) getDoubleFromStack( stackPointer ) ) ;
}
/*------------------------------------------------------------------------*/
