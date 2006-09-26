/*------------------------------------------------------------------------*/
/* file: set_figure_position_property.c                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the figure_position field of       */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"


/*------------------------------------------------------------------------*/
int set_figure_position_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  double * values = getDoubleMatrixFromStack( stackPointer ) ;
  return sciSetFigurePos( pobj, (int)values[0], (int)values[1]);
}
/*------------------------------------------------------------------------*/
