/*------------------------------------------------------------------------*/
/* file: set_isoview_property.c                                           */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the isoview field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_isoview_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"isoview") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"iso_view") ;
    return SET_PROPERTY_ERROR ;
  }
  if ( isStringParamEqual( stackPointer, "on" ) )
  {
    pSUBWIN_FEATURE (pobj)->isoview = TRUE ;
  }
  else if ( isStringParamEqual( stackPointer, "off" ) )
  {
    pSUBWIN_FEATURE (pobj)->isoview = FALSE ;
  }
  else
  {
    sciprint("Value must be 'on' or 'off'.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
