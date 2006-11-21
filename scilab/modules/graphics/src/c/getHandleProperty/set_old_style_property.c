/*------------------------------------------------------------------------*/
/* file: set_old_style_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the old_style field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_old_style_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  getStringFromStack( stackPointer ) ;

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint("Incompatible type for property old_style.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "on" ) )
  {
    setVersionFlag( 1 ) ;
    return SET_PROPERTY_SUCCEED ;
  }
  else if ( isStringParamEqual( stackPointer, "off" ) )
  {
    setVersionFlag( 0 ) ;
    return SET_PROPERTY_SUCCEED ;
  }
  else
  {
    sciprint("old_style must be 'on' or 'off'.\n");
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
