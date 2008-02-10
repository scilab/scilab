/*------------------------------------------------------------------------*/
/* file: set_font_color_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the font_color field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_font_color_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  int value = (int) getDoubleFromStack( stackPointer ) ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"font_color") ;
    return SET_PROPERTY_ERROR ;
  }

  if (   sciGetEntityType(pobj) == SCI_SUBWIN
           || sciGetEntityType(pobj) == SCI_FIGURE
           || sciGetEntityType(pobj) == SCI_AXES)
  {
    return sciSetFontForeground( pobj,value ) ;
  } /* F.Leray 08.04.04 */
  else
  {
    sciprint(_("%s property does not exist for this handle.\n"),"font_color") ;
    return -1 ;
  }
  return -1 ;
}
/*------------------------------------------------------------------------*/
