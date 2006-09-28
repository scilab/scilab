/*------------------------------------------------------------------------*/
/* file: set_text_property.c                                              */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the text field of                  */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_text_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  return sciSetText( pobj, getStringMatrixFromStack( stackPointer ), nbRow, nbCol ) ;
}
/*------------------------------------------------------------------------*/
