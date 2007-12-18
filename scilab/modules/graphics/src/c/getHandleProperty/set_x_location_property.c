/*------------------------------------------------------------------------*/
/* file: set_x_location_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the x_location field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_x_location_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  
  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"x_location") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"x_location") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "top" ) )
  {
    pSUBWIN_FEATURE(pobj)->axes.xdir = 'u' ;
  }
  else if ( isStringParamEqual( stackPointer, "bottom" ) )
  {
    pSUBWIN_FEATURE(pobj)->axes.xdir = 'd' ;
  }
  else if ( isStringParamEqual( stackPointer, "middle" ) )
  {
    pSUBWIN_FEATURE(pobj)->axes.xdir = 'c' ;
  }
  else  
  {
    sciprint("Second argument must be 'top', 'bottom' or 'middle'.\n") ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
