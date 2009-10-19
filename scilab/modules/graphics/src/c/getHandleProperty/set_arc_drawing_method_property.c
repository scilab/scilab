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
/* file: set_drawing_method_property.c                                    */
/* desc : function to modify in Scilab the drawing_method field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_arc_drawing_method_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( sciGetEntityType(pobj) != SCI_ARC && sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"arc_drawing_method") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( !isParameterStringMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "arc_drawing_method");
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "nurbs" ) )
  {
    return sciSetUseNurbs(pobj, TRUE) ;
  }
  else if ( isStringParamEqual( stackPointer, "lines" ) )
  {
    return sciSetUseNurbs(pobj, FALSE) ;
  }
  else
  {
    Scierror(999, _("Wrong value for '%s' property: %s or %s expected.\n"), "drawing_method", "nurbs", "lines");
    return SET_PROPERTY_ERROR ;
  }


}
/*------------------------------------------------------------------------*/
