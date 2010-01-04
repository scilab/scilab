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
/* file: set_outise_colors_property.c                                     */
/* desc : function to modify in Scilab the outside_colors field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_outside_colors_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  int  values[2];
	int nbColors = 0;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Integer expected.\n"), "outside_colors");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_FEC )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"outside_colors") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow * nbCol != 2 )
  {
    Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "outside_colors", 2);
    return SET_PROPERTY_ERROR ;
  }

	copyDoubleVectorToIntFromStack(stackPointer, values, 2);

	nbColors = sciGetNumColors(sciGetParentFigure(pobj));

	if (   values[0] > nbColors || values[0] < -1
		  || values[1] > nbColors || values[1] < -1)
	{
		/* It is possible to set color_range outside the colormap, however it won't be used.*/
		sciprint(_("WARNING: Wrong value for '%s' property: indices oustside the colormap will be clamped.\n"), "outside_colors");
	}


	return sciSetOutsideColors(pobj, values);
}
/*------------------------------------------------------------------------*/
