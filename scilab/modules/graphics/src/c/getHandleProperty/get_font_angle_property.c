/*------------------------------------------------------------------------*/
/* file: get_font_angle_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the font_angle field of a        */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"

/*------------------------------------------------------------------------*/
int get_font_angle_property( sciPointObj * pobj )
{
  return sciReturnDouble( RAD2DEG(sciGetFontOrientation(pobj)) ) ;
}
/*------------------------------------------------------------------------*/
