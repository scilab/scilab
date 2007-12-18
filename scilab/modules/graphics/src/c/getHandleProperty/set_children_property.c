/*------------------------------------------------------------------------*/
/* file: set_children_property.c                                          */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the children field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_children_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  sciprint("Children property can not be modified directly.\n") ;
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
