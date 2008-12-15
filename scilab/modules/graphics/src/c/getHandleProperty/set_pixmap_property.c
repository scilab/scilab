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
/* file: set_pixmap_property.c                                            */
/* desc : function to modify in Scilab the pixmap field of                */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "GetProperty.h"

/*------------------------------------------------------------------------*/
int set_pixmap_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  if ( sciGetEntityType (pobj) != SCI_FIGURE )
  {
    Scierror(999, _("%s property undefined for this object.\n"), "pixmap") ;
    return -1;
  }

  if ( isStringParamEqual( stackPointer, "on" ) )
  {
    sciSetPixmapMode(pobj, TRUE) ;
  }
  else if ( isStringParamEqual( stackPointer, "off" ) )
  {
    sciSetPixmapMode(pobj, FALSE);
  }
  else
  {
	  Scierror(999, _("Wrong value for argument: '%s' or '%s' expected.\n"),"on","off");
    return -1 ;
  }
  return 0 ;
}
/*------------------------------------------------------------------------*/
