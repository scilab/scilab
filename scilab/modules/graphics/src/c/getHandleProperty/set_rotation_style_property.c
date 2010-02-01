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
/* file: set_rotation_style_property.c                                    */
/* desc : function to modify in Scilab the rotation_style field of        */
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
int set_rotation_style_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  getStringFromStack( stackPointer ) ;

  if ( !isParameterStringMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "rotation_style");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType (pobj) != SCI_FIGURE ) 
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"rotation_style");
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "unary" ) )
  {
    pFIGURE_FEATURE(pobj)->rotstyle = 0 ;
    return SET_PROPERTY_SUCCEED ;
  }
  else if ( isStringParamEqual( stackPointer, "multiple" ) )
  {
    pFIGURE_FEATURE(pobj)->rotstyle = 1 ;
    return SET_PROPERTY_SUCCEED ;
  }
  else
  {

    Scierror(999, _("Wrong value for '%s' property: %s or %s expected.\n"), "rotation_style", "'unary'", "'multiple'");
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
