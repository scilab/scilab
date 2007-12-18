/*------------------------------------------------------------------------*/
/* file: set_format_n_property.c                                          */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the format_n field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_format_n_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"format_n") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_AXES )
  {
    sciprint( "format_n property does not exists for this handle" ) ;
    return SET_PROPERTY_ERROR ;
  }
  pAXES_FEATURE(pobj)->format[0] = getStringFromStack(stackPointer)[0] ;
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
