/*------------------------------------------------------------------------*/
/* file: set_figure_position_property.c                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the screen_position field of       */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_screen_position_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  double * values = getDoubleMatrixFromStack( stackPointer ) ;
 
  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"figure_position") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow * nbCol != 2 )
  {
    sciprint("Wrong size for screen_position property, argument should be a vector of size 2.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  return sciSetScreenPosition( pobj, (int)values[0], (int)values[1]);
}
/*------------------------------------------------------------------------*/
