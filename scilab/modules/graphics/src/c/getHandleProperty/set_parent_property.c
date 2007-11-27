/*------------------------------------------------------------------------*/
/* file: set_parent_property.c                                            */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the parent field of                */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_parent_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  if(sciGetEntityType( pobj ) != SCI_MENU) {
    return setMenuParent(pobj, stackPointer, valueType, nbRow, nbCol);
  }
  else
    {
      sciprint( "Parent property can not be modified directly.\n" ) ;
      return SET_PROPERTY_ERROR ;
    }
}
/*------------------------------------------------------------------------*/
