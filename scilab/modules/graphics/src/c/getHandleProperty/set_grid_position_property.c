/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_grid_position_property.c                                     */
/* desc : function to modify in Scilab the grid_position field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_grid_position_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

	if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"grid_position") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( !isParameterStringMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "grid_position");
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "foreground" ) )
  {
		sciSetGridFront(pobj, TRUE);
  }
  else if ( isStringParamEqual( stackPointer, "background" ) )
  {
    sciSetGridFront(pobj, FALSE);
  }
  else
  {
    Scierror(999, _("Wrong value for '%s' property: %s or %s expected.\n"), "grid_position", "foreground", "background");
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
