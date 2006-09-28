/*------------------------------------------------------------------------*/
/* file: set_fill_mode_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the fill_mode field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_fill_mode_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  if ( isStringParamEqual( stackPointer, "on" ) )
  {
    return sciSetIsFilled( pobj, TRUE ) ;
  }
  else if ( isStringParamEqual( stackPointer, "off" ) )
  {
    return sciSetIsFilled( pobj, FALSE ) ;
  }
  else
  {
    sciprint( "Nothing to do (value must be 'on/off').\n" ) ;
    return -1;
  }
  return -1 ;
}
/*------------------------------------------------------------------------*/
