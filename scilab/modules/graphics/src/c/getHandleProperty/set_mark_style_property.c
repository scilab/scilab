/*------------------------------------------------------------------------*/
/* file: set_mark_style_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the mark_style field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_mark_style_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  int status1 = sciSetIsMark( pobj, TRUE ) ;
  int status2 = sciSetMarkStyle( pobj, (int) getDoubleFromStack(stackPointer) ) ;

  return sciSetFinalStatus( status1, status2 ) ;
}
/*------------------------------------------------------------------------*/
