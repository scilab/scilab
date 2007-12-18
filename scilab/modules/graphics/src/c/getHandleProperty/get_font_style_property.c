/*------------------------------------------------------------------------*/
/* file: get_font_style_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the font_style field of a        */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_font_style_property( sciPointObj * pobj )
{
  return sciReturnInt( sciGetFontStyle( pobj ) ) ;
}
/*------------------------------------------------------------------------*/
