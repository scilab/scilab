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
/* file: set_callback_property.c                                          */
/* desc : function to modify in Scilab the callback field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/
#include "stack-c.h"
#include "setHandleProperty.h"
#include "getPropertyAssignedValue.h"
#include "Interaction.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
/*------------------------------------------------------------------------*/
int set_callback_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  if (sciGetEntityType(pobj) == SCI_UIMENU || sciGetEntityType(pobj) == SCI_UICONTROL)
    {
      if ( (!isParameterStringMatrix( valueType )) && (valueType != sci_list) )
        {
          Scierror(999, _("Wrong type for '%s' property: String matrix expected.\n"), "callback");
          return SET_PROPERTY_ERROR ;
        }
      return SetUiobjectCallback(pobj, stackPointer, valueType, nbRow, nbCol);
    }
  else
    {
      if ( !isParameterStringMatrix( valueType ) )
        {
          Scierror(999, _("Wrong type for '%s' property: String matrix expected.\n"), "callback");
          return SET_PROPERTY_ERROR ;
        }
     return sciAddCallback( pobj, getStringFromStack( stackPointer ), nbRow * nbCol, 1 ) ;
    }
}
/*------------------------------------------------------------------------*/
