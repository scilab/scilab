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
/* file: set_anti_aliasing_property.c                                     */
/* desc : function to modify in Scilab the anti_aliasing field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "GraphicSynchronizerInterface.h"

/*------------------------------------------------------------------------*/
int set_anti_aliasing_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

	int quality = 0;
	int status;

  if ( !isParameterStringMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "anti_aliasing");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_FIGURE )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"), "anti_aliasing") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "off" ) )
  {
		quality = 0;
  }
  else if ( isStringParamEqual( stackPointer, "2x" ) )
  {
    quality = 2;
  }
	else if ( isStringParamEqual( stackPointer, "4x" ) )
  {
    quality = 4;
  }
	else if ( isStringParamEqual( stackPointer, "8x" ) )
  {
    quality = 8;
  }
	else if ( isStringParamEqual( stackPointer, "16x" ) )
  {
    quality = 16;
  }
  else
  {
    Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "anti_aliasing", "off, 2x, 4x, 8x, 16x");
    return SET_PROPERTY_ERROR ;
  }

	/* Modifying the antialaising may ask for a redraw */
	disableFigureSynchronization(pobj);
	status = sciSetAntialiasingQuality(pobj, quality);
	enableFigureSynchronization(pobj);

	return status;
}
/*------------------------------------------------------------------------*/
