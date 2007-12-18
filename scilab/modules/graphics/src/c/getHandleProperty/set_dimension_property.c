/*------------------------------------------------------------------------*/
/* file: set_figure_position_property.c                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : set the dimension (width, height) in pixels of a graphical      */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_dimension_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  double * values = getDoubleMatrixFromStack( stackPointer ) ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"figure_position") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow * nbCol != 2 )
  {
    sciprint("Wrong size for dimension property, argument should be a vector of size 2.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  return sciSetDimension( pobj, (int)values[0], (int)values[1] ) ;
}
/*------------------------------------------------------------------------*/
