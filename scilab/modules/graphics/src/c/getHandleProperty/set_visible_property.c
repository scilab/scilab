/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_visible_property.c                                           */
/* desc : function to modify in Scilab the visible field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "SetUiobjectVisible.h"
#include "GetProperty.h"

/*------------------------------------------------------------------------*/
int set_visible_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  getStringFromStack( stackPointer ) ;

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"visible") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( (sciGetEntityType(pobj) == SCI_UIMENU) || (sciGetEntityType(pobj) == SCI_UICONTROL) )
    {
      return SetUiobjectVisible(pobj, stackPointer, valueType, nbRow, nbCol);
    }

  if ( isStringParamEqual( stackPointer, "on" ) )
  {
    return sciSetVisibility( pobj, TRUE ) ;
  }
  else if ( isStringParamEqual( stackPointer, "off" ) )
  {
    return sciSetVisibility( pobj, FALSE ) ;
  }
  else
  {
    sciprint(_("Wrong value: '%s' or '%s' expected.\n"),"on","off") ;
    return SET_PROPERTY_ERROR ;
  }
}
/*------------------------------------------------------------------------*/
