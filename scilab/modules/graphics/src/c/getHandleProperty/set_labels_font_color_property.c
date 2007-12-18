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

/*------------------------------------------------------------------------*/
int set_labels_font_color_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"labels_font_color") ;
    return SET_PROPERTY_ERROR ;
  }

  if (sciGetEntityType (pobj) == SCI_AXES)
  {
    pAXES_FEATURE(pobj)->textcolor = (int) getDoubleFromStack( stackPointer ) ;
  }
  else if ( sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE )
  {
    return sciSetFontForeground( pobj, (int) getDoubleFromStack( stackPointer ) ) ;
  } /* F.Leray 08.04.04 */
  else
  {
    sciprint( "labels_font_color property does not exist for this handle.\n" ) ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
