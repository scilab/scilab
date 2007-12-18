/*------------------------------------------------------------------------*/
/* file: set_mark_size_unit_property.c                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the mark_size_unit field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_mark_size_unit_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"mark_size_unit") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "point") )
  {
    return sciSetMarkSizeUnit( pobj, 1 ) ; /* 1 : points, 2 : tabulated */
  }
  else if ( isStringParamEqual( stackPointer, "tabulated" ) )
  {
    return sciSetMarkSizeUnit( pobj, 2) ;
  }
  else
  {
    sciprint("Value must be 'point/tabulated'.\n") ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
