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
/* file: set_auto_resize_property.c                                       */
/* desc : function to modify in Scilab the auto_resize field of           */
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
int set_auto_resize_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  int status = SET_PROPERTY_ERROR;
  if ( !isParameterStringMatrix( valueType ) )
  {
    Scierror(999, _("Incompatible type for property %s.\n"),"auto_resize") ;
    return SET_PROPERTY_ERROR ;
  }

	if ( sciGetEntityType(pobj) != SCI_FIGURE )
  {
    Scierror(999, _("%s undefined for this object.\n"), "auto_resize") ;
    return SET_PROPERTY_ERROR ;
  }

  /* disable protection since this function will call Java */
  disableFigureSynchronization(pobj);
  if ( isStringParamEqual( stackPointer, "on" ) )
  {
    status = sciSetResize( pobj, TRUE );
  }
  else if ( isStringParamEqual( stackPointer, "off" ) )
  {
    status = sciSetResize( pobj, FALSE );
  }
  else
  {
    Scierror(999, _("%s: Wrong input argument: '%s' or '%s' expected.\n"),"set_auto_resize_property","on","off");
    return SET_PROPERTY_ERROR ;
  }
  enableFigureSynchronization(pobj);
  /* return set property unchanged since repaint is not really needed */
	return sciSetNoRedrawStatus((SetPropertyStatus)status);
}
/*------------------------------------------------------------------------*/
