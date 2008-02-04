/*------------------------------------------------------------------------*/
/* file: set_callback_type_property.c                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the callback_type field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_callback_type_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  int cbType = -1;

  if ( !isParameterDoubleMatrix(valueType) || nbRow !=1 || nbCol != 1 )
  {
    sciprint(_("Incompatible value for property %s: must be -1, 0, 1 or 2.\n"),"callback_type") ;
    return SET_PROPERTY_ERROR ;
  }

  cbType = getDoubleFromStack(stackPointer);

  if (cbType < -1 || cbType > 2)
    {
      sciprint(_("Incompatible value for property %s: must be -1, 0, 1 or 2.\n"),"callback_type") ;
      return SET_PROPERTY_ERROR ;
    }

  if (sciGetEntityType (pobj) == SCI_UIMENU)
    {
      pUIMENU_FEATURE(pobj)->callbackType = cbType;
    }
  else if (sciGetEntityType (pobj) == SCI_UICONTROL)
    {
      pUICONTROL_FEATURE(pobj)->callbackType = cbType;
    }
  else
    {
      sciprint(_("%s property does not exist for this handle.\n"),"callback_type");
      return SET_PROPERTY_ERROR ;
    }

  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
