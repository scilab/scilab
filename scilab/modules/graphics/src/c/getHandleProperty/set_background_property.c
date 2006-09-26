/*------------------------------------------------------------------------*/
/* file: set_background_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the background field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"

/*------------------------------------------------------------------------*/
int set_background_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  return sciSetBackground( pobj, (int)getDoubleFromStack(stackPointer) ) ;
}
/*------------------------------------------------------------------------*/
