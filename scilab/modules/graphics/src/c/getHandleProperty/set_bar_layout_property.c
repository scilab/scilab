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
/* file: set_bar_layout_property.c                                        */
/* desc : function to modify in Scilab the bar_layout field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_bar_layout_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "bar_layout");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_POLYLINE )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"bar_layout") ;
    return SET_PROPERTY_ERROR ;
  }
  
  if( isStringParamEqual( stackPointer, "grouped" ) )
  {
    pPOLYLINE_FEATURE (pobj)->bar_layout = 0 ;
  }
  else if( isStringParamEqual( stackPointer, "stacked" ) )
  {
    pPOLYLINE_FEATURE (pobj)->bar_layout = 1 ;
  }
  else
  {
    Scierror(999, _("Wrong value for '%s' property: %s or %s expected.\n"), "bar_layout", "grouped", "stacked");
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
