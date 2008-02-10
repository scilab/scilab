/*------------------------------------------------------------------------*/
/* file: set_labels_font_color_property.c                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the labels_font_color field of     */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_labels_font_color_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"labels_font_color") ;
    return SET_PROPERTY_ERROR ;
  }

  if (   sciGetEntityType (pobj) == SCI_SUBWIN
      || sciGetEntityType (pobj) == SCI_FIGURE
      || sciGetEntityType (pobj) == SCI_AXES)
  {
    return sciSetFontForeground( pobj, (int) getDoubleFromStack( stackPointer ) ) ;
  } /* F.Leray 08.04.04 */
  else
  {
    sciprint(_("%s property does not exist for this handle.\n"),"labels_font_color") ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
