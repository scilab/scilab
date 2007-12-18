/*------------------------------------------------------------------------*/
/* file: set_bar_width_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the bar_width field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_bar_width_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"bar_width") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType (pobj) != SCI_POLYLINE )
  {
    sciprint( "Object has no bar shift.\n" ) ;
    return SET_PROPERTY_ERROR ;
  }
  pPOLYLINE_FEATURE (pobj)->bar_width = getDoubleFromStack( stackPointer ) ;
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
