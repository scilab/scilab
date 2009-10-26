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
/* file: get_anti_aliasing_property.c                                     */
/* desc : function to retrieve in Scilab the anti_aliasing field of a     */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_anti_aliasing_property( sciPointObj * pobj )
{

  if ( sciGetEntityType (pobj) != SCI_FIGURE )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"anti_aliasing");
    return -1;
  }

  switch(sciGetAntialiasingQuality(pobj))
	{
	case 0:
		return sciReturnString("off");
		break;
	case 2:
		return sciReturnString("2x");
		break;
	case 4:
		return sciReturnString("4x");
		break;
	case 8:
		return sciReturnString("8x");
		break;
	case 16:
		break;
		return sciReturnString("16x");
	default:
    Scierror(999, _("Wrong value for '%s' property.\n"),"anti_aliasing");
    return -1 ;
		break;
	}

	return sciReturnString("off");

}
/*------------------------------------------------------------------------*/
