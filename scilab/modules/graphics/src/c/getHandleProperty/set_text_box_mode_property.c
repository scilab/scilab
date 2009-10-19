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
/* file: set_text_box_mode_property.c                                     */
/* desc : function to modify in Scilab the text_box_mode field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_text_box_mode_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  
  if ( !isParameterStringMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "text_box_mode");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_TEXT )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"text_box_mode") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "off" ) )
  {
    int status1 = sciSetCenterPos( pobj, FALSE ) ;
    int status2 = sciSetAutoSize(  pobj, TRUE  ) ;
    return sciSetFinalStatus( (SetPropertyStatus)status1, (SetPropertyStatus)status2 ) ;
  }
  else if ( isStringParamEqual( stackPointer, "centered" ) )
  {
    int status1 = sciSetCenterPos( pobj, TRUE  ) ;
    int status2 = sciSetAutoSize(  pobj, TRUE  ) ;
    return sciSetFinalStatus( (SetPropertyStatus)status1, (SetPropertyStatus)status2 ) ;
  }
  else if ( isStringParamEqual( stackPointer, "filled" ) )
  {
    int status1 = sciSetCenterPos( pobj, TRUE  ) ;
    int status2 = sciSetAutoSize(  pobj, FALSE ) ;
    return sciSetFinalStatus( (SetPropertyStatus)status1, (SetPropertyStatus)status2 ) ;
  }
  else
  {
    Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "text_box_mode", "off, centered, filled");
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;

}
/*------------------------------------------------------------------------*/
