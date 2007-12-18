/*------------------------------------------------------------------------*/
/* file: get_labels_font_style_property.c                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the labels_font_style field of   */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_labels_font_style_property( sciPointObj * pobj )
{
  if ( sciGetEntityType( pobj ) != SCI_SUBWIN && sciGetEntityType( pobj ) != SCI_FIGURE )
  {
    sciprint(_("%s does not exist for this handle.\n"), "labels_font_style property") ;
    return -1 ;
  }

  if ( sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE )
  { 
    /* F.Leray 09.04.04: For the moment sciAxes have no font_style property*/
    return sciReturnDouble( sciGetFontStyle( pobj ) ) ;
  }
  return -1 ;
}
/*------------------------------------------------------------------------*/
