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
#include "localization.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "InitUIMenu.h"
#include "SetUicontrolParent.h"

/*------------------------------------------------------------------------*/
int set_parent_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  if(sciGetEntityType( pobj ) == SCI_UIMENU)
    {
      return setMenuParent(pobj, stackPointer, valueType, nbRow, nbCol);
    }
  else if(sciGetEntityType(pobj) == SCI_UICONTROL)
    {
      return SetUicontrolParent(pobj, stackPointer, valueType, nbRow, nbCol);
    }
  else
    {
      sciprint("Parent property can not be modified directly.\n") ;
      return SET_PROPERTY_ERROR ;
    }
}
/*------------------------------------------------------------------------*/
