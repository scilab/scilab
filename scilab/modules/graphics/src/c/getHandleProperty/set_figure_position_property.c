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
#include "sciprint.h"
#include "localization.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_figure_position_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  return set_screen_position_property( pobj, stackPointer, valueType, nbRow, nbCol ) ;
}
/*------------------------------------------------------------------------*/
