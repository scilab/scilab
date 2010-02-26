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
/* file: set_default_values_property.c                                    */
/* desc : function to modify in Scilab the default_values field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "InitObjects.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_default_values_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real expected.\n"), "default_values");
    return SET_PROPERTY_ERROR ;
  }

  if ( getDoubleFromStack( stackPointer ) != 1 )
  {
    Scierror(999, _("Wrong value for '%s' property: Must be '%s'.\n"), "default_values", "1");
    return SET_PROPERTY_ERROR ;
  }

  if ( pobj == getFigureModel() )
  {
    return InitFigureModel();
  }
  else if ( pobj == getAxesModel() )
  {
    return InitAxesModel();
  }
  else if (pobj == NULL)
  {
		/* set default values for current figure */
    return sciSetDefaultValues();
  }

	Scierror(999, _("'%s' property does not exist for this handle.\n"), "default_values");
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
