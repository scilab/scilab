/*------------------------------------------------------------------------*/
/* file: set_callback_type_property.c                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the callback_type field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_callback_type_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"callback_type") ;
    return SET_PROPERTY_ERROR ;
  }

  if (sciGetEntityType (pobj) != SCI_UIMENU)
  {
    sciprint("callback_type property does not exist for this handle.\n");
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "string" ) )
  {
    pUIMENU_FEATURE(pobj)->CallbackType = 0 ;
  }
  else if ( isStringParamEqual( stackPointer, "C" ) )
  {
    pUIMENU_FEATURE(pobj)->CallbackType = 1 ;
  }
  else if ( isStringParamEqual( stackPointer, "internal" ) )
  {
    pUIMENU_FEATURE(pobj)->CallbackType = 2 ;
  }
  else if ( isStringParamEqual( stackPointer, "addmenu" ) )
  {
    pUIMENU_FEATURE(pobj)->CallbackType = 3 ;
  }
  else
  {
    sciprint("Value must be 'string','C','internal','addmenu'.\n" ) ;
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
