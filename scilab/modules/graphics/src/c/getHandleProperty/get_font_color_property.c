/*------------------------------------------------------------------------*/
/* file: get_font_color_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the font_color field of a        */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_font_color_property( sciPointObj * pobj )
{
  if (   sciGetEntityType(pobj) == SCI_SUBWIN
      || sciGetEntityType(pobj) == SCI_FIGURE
      || sciGetEntityType(pobj) == SCI_AXES)
  {
    return sciReturnDouble(  sciGetFontForegroundToDisplay(pobj) ) ;
  }
  else
  {
    sciprint(_("%s property does not exist for this handle.\n"),"Font_color") ;
    return -1 ;
  }
}
/*------------------------------------------------------------------------*/
