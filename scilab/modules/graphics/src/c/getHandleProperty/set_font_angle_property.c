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
#include "sciprint.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_font_angle_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint("Incompatible type for property font_angle.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetAutoRotation( pobj ) )
  {
    sciSetAutoRotation( pobj, FALSE ) ;
  }
  return sciSetFontOrientation( pobj, DEG2RAD(getDoubleFromStack(stackPointer))  ) ;

}
/*------------------------------------------------------------------------*/
