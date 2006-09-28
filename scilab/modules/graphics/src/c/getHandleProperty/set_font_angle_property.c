/*------------------------------------------------------------------------*/
/* file: set_font_angle_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the font_angle field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "GetProperty.h"

/*------------------------------------------------------------------------*/
int set_font_angle_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  if ( sciGetAutoRotation( pobj ) )
  {
    sciSetAutoRotation( pobj, FALSE ) ;
  }
  return sciSetFontOrientation( pobj,(int) ( getDoubleFromStack(stackPointer) * 10 ) ) ;

}
/*------------------------------------------------------------------------*/
