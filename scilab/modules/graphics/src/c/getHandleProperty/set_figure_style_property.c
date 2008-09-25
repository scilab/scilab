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
/* file: set_figure_style_property.c                                      */
/* desc : function to modify in Scilab the figure_style field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "InitObjects.h"
#include "DrawObjects.h"
#include "DestroyObjects.h"
#include "BuildObjects.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_figure_style_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"figure_style") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "old" ) )
  {
    sciprint(_("Old graphic mode is no longer available. Please refer to the set help page.\n")) ;
    return SET_PROPERTY_ERROR ;
  }
  else if ( isStringParamEqual( stackPointer, "new" ) )
  {
    /* graphic mode must be new */
    /* nothing to do */
    return SET_PROPERTY_UNCHANGED ;
  }
  else
  {
    sciprint(_("Wrong value for argument: '%s' or '%s' expected.\n"),"old","new");
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
