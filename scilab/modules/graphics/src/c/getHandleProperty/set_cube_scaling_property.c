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
/* file: set_cube_scaling_property.c                                      */
/* desc : function to modify in Scilab the cube_scaling field of          */
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
int set_cube_scaling_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"cube_scaling") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"cube_scaling") ;
    return SET_PROPERTY_ERROR ;
  }

  if( !sciGetIs3d(pobj) )
  {
    sciprint(_("Warning: %s property is only used in 3D mode.\n"),"cube_scaling");
  }

  if ( isStringParamEqual(stackPointer, "on" ) )
  {
    pSUBWIN_FEATURE (pobj)->cube_scaling = TRUE ;
  }
  else if ( isStringParamEqual( stackPointer, "off") )
  {
    pSUBWIN_FEATURE (pobj)->cube_scaling = FALSE ;
  }
  else
  {
    sciprint(_("Wrong value for argument: '%s' or '%s' expected.\n"),"on","off");
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
