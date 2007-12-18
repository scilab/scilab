/*------------------------------------------------------------------------*/
/* file: set_rotation_style_property.c                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the rotation_style field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_rotation_style_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  getStringFromStack( stackPointer ) ;

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"rotation_style") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType (pobj) != SCI_FIGURE ) 
  {
    sciprint( "rotation_style_property undefined for this object.\n" ) ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "unary" ) )
  {
    pFIGURE_FEATURE(pobj)->rotstyle = 0 ;
    return SET_PROPERTY_SUCCEED ;
  }
  else if ( isStringParamEqual( stackPointer, "multiple" ) )
  {
    pFIGURE_FEATURE(pobj)->rotstyle = 1 ;
    return SET_PROPERTY_SUCCEED ;
  }
  else
  {
    sciprint("Nothing to do (value must be 'unary/multiple').\n");
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
