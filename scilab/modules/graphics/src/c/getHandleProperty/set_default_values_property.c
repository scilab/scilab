/*------------------------------------------------------------------------*/
/* file: set_default_values_property.c                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the default_values field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "InitObjects.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_default_values_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"default_values") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( getDoubleFromStack( stackPointer ) != 1 )
  {
    sciprint( "Value must be 1 to set default values.\n" ) ;
    return SET_PROPERTY_ERROR ;
  }

  if ( pobj == getFigureModel() )
  {
    return InitFigureModel();
  }
  else if ( pobj == getAxesModel() )
  {
    return InitAxesModel();
  }
  else
  {
    return sciSetDefaultValues();
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
