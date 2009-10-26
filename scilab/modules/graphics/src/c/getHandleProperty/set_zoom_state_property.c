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
/* file: set_zoom_state_property.c                                        */
/* desc : function to modify in Scilab the zoom_state field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "PloEch.h"
#include "BOOL.h"
/*------------------------------------------------------------------------*/
int set_zoom_state_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
	int b = (int)FALSE;
	if ( sciGetEntityType(pobj) != SCI_SUBWIN )
	{
		Scierror(999, _("'%s' property does not exist for this handle.\n"),"zoom_state") ;
		return SET_PROPERTY_ERROR ;
	}

	b = tryGetBooleanValueFromStack(stackPointer, valueType, nbRow, nbCol, "zoom_state");
	if(b == NOT_A_BOOLEAN_VALUE) return SET_PROPERTY_ERROR;

	if(b)
	{ 
		if(sciGetZooming(pobj))
		{
			Scierror(999, "Object is already zoomed.\n");
			return SET_PROPERTY_ERROR;
		}
		else
		{
			Scierror(999, "set zoom box ( set('zoom_box',[xmin ymin xmax ymax])).\n");
			return SET_PROPERTY_ERROR;
		}
	}
	else
	{ 
		unzoom();
		return sciSetZooming(pobj, FALSE);
	}

	return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
