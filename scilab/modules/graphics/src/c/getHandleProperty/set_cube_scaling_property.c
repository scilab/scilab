/*------------------------------------------------------------------------*/
/* file: set_cube_scaling_property.c                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the cube_scaling field of          */
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
int set_cube_scaling_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"cube_scaling") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"cube_scaling") ;
    return SET_PROPERTY_ERROR ;
  }

  if( !sciGetIs3d(pobj) )
  {
    sciprint("Warning: cube_scaling property is only used in 3D mode.\n");
  }

  if ( isStringParamEqual(stackPointer, "on" ) )
  {
    pSUBWIN_FEATURE (pobj)->cube_scaling = TRUE ;
  }
  else if ( isStringParamEqual( stackPointer, "off") )
  {
    pSUBWIN_FEATURE (pobj)->cube_scaling = FALSE ;
  }
  else
  {
    sciprint("Value must be 'on' or 'off' / Only use for 3D mode.\n") ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
