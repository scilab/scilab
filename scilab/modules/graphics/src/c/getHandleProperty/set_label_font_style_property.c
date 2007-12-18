/*------------------------------------------------------------------------*/
/* file: set_labels_font_style_property.c                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the labels_font_style field of     */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_labels_font_style_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"labels_font_style") ;
    return SET_PROPERTY_ERROR ;
  }

  if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE)
  {
    return sciSetFontStyle( pobj, (int) getDoubleFromStack(stackPointer) ) ;
  }
  else
  {
    sciprint(_("%s property does not exist for this handle.\n"),"labels_font_style") ;
    return SET_PROPERTY_ERROR ;
  }
}
/*------------------------------------------------------------------------*/
