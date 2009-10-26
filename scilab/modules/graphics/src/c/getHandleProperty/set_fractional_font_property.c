/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
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
/* file: set_fractional_fonts_property.c                                  */
/* desc : function to modify in Scilab the fractional_font field of       */
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
int set_fractional_font_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
	int b =  (int)FALSE;
	if (   sciGetEntityType(pobj) != SCI_SUBWIN
		&& sciGetEntityType(pobj) != SCI_TEXT
		&& sciGetEntityType(pobj) != SCI_LABEL
		&& sciGetEntityType(pobj) != SCI_AXES
		&& sciGetEntityType(pobj) != SCI_LEGEND)
	{
		Scierror(999, _("'%s' property does not exist for this handle.\n"),"fractional_font");
		return SET_PROPERTY_ERROR ;
	}

	b =  tryGetBooleanValueFromStack(stackPointer, valueType, nbRow, nbCol, "fractional_font");
	if(b == NOT_A_BOOLEAN_VALUE) return SET_PROPERTY_ERROR;

	sciSetIsUsingFractionalMetrics(pobj, b);

	return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
