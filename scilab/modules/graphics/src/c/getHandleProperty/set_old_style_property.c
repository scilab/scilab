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
#include "localization.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_old_style_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  getStringFromStack( stackPointer ) ;

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"old_style") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "off" ) )
  {
    return SET_PROPERTY_UNCHANGED ;
  }
  else if ( isStringParamEqual( stackPointer, "on" ) )
  {
    sciprint("Old graphic mode is no longer available. Please refer to the set help page.\n") ;
    return SET_PROPERTY_ERROR ;
  }
  else
  {
    sciprint("old_style must be 'on' or 'off'.\n");
    return SET_PROPERTY_ERROR ;
  }
}
/*------------------------------------------------------------------------*/
