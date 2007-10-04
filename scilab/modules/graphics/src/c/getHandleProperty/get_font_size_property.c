/*------------------------------------------------------------------------*/
/* file: get_font_size_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the font_size field of a         */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"

/*------------------------------------------------------------------------*/
int get_font_size_property( sciPointObj * pobj )
{
  return sciReturnDouble(sciGetFontSize(pobj) ) ;
}
/*------------------------------------------------------------------------*/
