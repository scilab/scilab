/*------------------------------------------------------------------------*/
/* file: set_callback_property.c                                          */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the callback field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Interaction.h"
#include "sciprint.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_callback_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
   return sciAddCallback( pobj, getStringFromStack( stackPointer ), nbRow * nbCol, 1 ) ;
}
/*------------------------------------------------------------------------*/
