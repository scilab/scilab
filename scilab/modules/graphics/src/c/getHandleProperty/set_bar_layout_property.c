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
/* file: set_bar_layout_property.c                                        */
/* desc : function to modify in Scilab the bar_layout field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_bar_layout_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"bar_layout") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_POLYLINE )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"bar_layout") ;
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
    sciprint("bar_layout must be set to 'grouped' or 'stacked'.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
