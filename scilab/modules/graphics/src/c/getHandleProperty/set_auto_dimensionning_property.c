/*------------------------------------------------------------------------*/
/* file: set_auto_dimensionning_property.c                                */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the auto_dimensionning field of    */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_auto_dimensionning_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"auto_dimensionning") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType( pobj ) != SCI_TEXT )
  {
    sciprint( "auto_dimensionning property does not exist for this handle.\n" ) ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "on" ) )
  {
    return sciSetAutoSize( pobj, TRUE ) ;
  }
  else if ( isStringParamEqual( stackPointer, "off" ) )
  {
    return sciSetAutoSize( pobj, FALSE ) ;
  }
  else
  {
    sciprint("Value must be 'on/off'.\n");
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;

}
/*------------------------------------------------------------------------*/
