/*------------------------------------------------------------------------*/
/* file: set_surface_mode_property.c                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the surface_mode field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "GetProperty.h"

/*------------------------------------------------------------------------*/
int set_surface_mode_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  
  if ( sciGetEntityType(pobj) != SCI_PLOT3D &&
       sciGetEntityType(pobj) != SCI_FAC3D  &&
       sciGetEntityType(pobj) != SCI_SURFACE   )
  {
    sciprint( "Surface_mode can not be set with this object, use line_mode.\n" ) ;
    return -1 ;
  }

  if ( isStringParamEqual( stackPointer, "on" ) )
  {
    return sciSetIsLine( pobj, TRUE ) ;
  }
  else if ( isStringParamEqual( stackPointer, "off" ) )
  {
    return sciSetIsLine( pobj, FALSE ) ;
  }
  else
  {
    sciprint( "Value must be 'on/off'.\n" ) ;
    return -1 ;
  }
  return -1 ;
}
/*------------------------------------------------------------------------*/
