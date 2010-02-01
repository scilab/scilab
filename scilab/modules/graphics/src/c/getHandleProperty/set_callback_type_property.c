/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_callback_type_property.c                                     */
/* desc : function to modify in Scilab the callback_type field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_callback_type_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  int cbType = -1;

  if ( !isParameterDoubleMatrix(valueType) || nbRow !=1 || nbCol != 1 )
  {
    Scierror(999, _("Wrong type for '%s' property: Integer expected.\n"), "callback_type");
    return SET_PROPERTY_ERROR ;
  }

  cbType = (int)getDoubleFromStack(stackPointer);

  if (cbType < -1 || cbType > 2)
    {
      Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "callback_type", "-1, 0, 1, 2");
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
      Scierror(999, _("'%s' property does not exist for this handle.\n"),"callback_type");
      return SET_PROPERTY_ERROR ;
    }

  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
