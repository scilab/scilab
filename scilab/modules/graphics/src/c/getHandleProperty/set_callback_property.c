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
#include "InitUIMenu.h"
#include "GetProperty.h"
/*------------------------------------------------------------------------*/
int set_callback_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"callback") ;
    return SET_PROPERTY_ERROR ;
  }

  if (sciGetEntityType(pobj) == SCI_UIMENU)
    {
      return setMenuCallback(pobj, stackPointer, valueType, nbRow, nbCol);
    }
  else
    {
      return sciAddCallback( pobj, getStringFromStack( stackPointer ), nbRow * nbCol, 1 ) ;
    }
}
/*------------------------------------------------------------------------*/
