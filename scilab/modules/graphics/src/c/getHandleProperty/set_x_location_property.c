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
/* file: set_x_location_property.c                                        */
/* desc : function to modify in Scilab the x_location field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_x_location_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  
  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"x_location") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"x_location") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "top" ) )
  {
    pSUBWIN_FEATURE(pobj)->axes.xdir = 'u' ;
  }
  else if ( isStringParamEqual( stackPointer, "bottom" ) )
  {
    pSUBWIN_FEATURE(pobj)->axes.xdir = 'd' ;
  }
  else if ( isStringParamEqual( stackPointer, "middle" ) || isStringParamEqual( stackPointer, "origin" ) )
  {
    /* old value middle kept for compatibility with scilab 4 */
    pSUBWIN_FEATURE(pobj)->axes.xdir = 'c' ;
  }
  else  
  {
    sciprint(_("%s: Wrong type for second input argument: '%s', '%s' or '%s' expected.\n"), "set_x_location_property","top","bottom","origin") ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
