/*------------------------------------------------------------------------*/
/* file: set_font_style_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the font_style field of            */
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
int set_font_style_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  int value = (int) getDoubleFromStack( stackPointer ) ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"font_style") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( value > 10 || value < 0 )
  {
    sciprint("The value must be in [0 10].\n") ;
    return SET_PROPERTY_ERROR ;
  }
  
  return sciSetFontStyle( pobj, value ) ;
}
/*------------------------------------------------------------------------*/
