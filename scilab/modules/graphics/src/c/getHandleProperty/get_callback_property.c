/*------------------------------------------------------------------------*/
/* file: get_callback_property.c                                          */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the callback field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "Interaction.h"
#include "returnProperty.h"

/*------------------------------------------------------------------------*/
int get_callback_property( sciPointObj * pobj )
{
  if(sciGetEntityType(pobj) == SCI_UIMENU || sciGetEntityType(pobj) == SCI_UICONTROL)
    {
      return GetUiobjectCallback(pobj);
    }
  else
    {
      return sciReturnString( sciGetCallback( pobj ) ) ;
    }
}
/*------------------------------------------------------------------------*/
