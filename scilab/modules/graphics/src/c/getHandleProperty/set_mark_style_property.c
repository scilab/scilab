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
/* file: set_mark_style_property.c                                        */
/* desc : function to modify in Scilab the mark_style field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_mark_style_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  int status1 ;
  int status2 ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Integer expected.\n"), "mark_style");
    return SET_PROPERTY_ERROR ;
  }

   status1 = sciSetIsMark( pobj, TRUE ) ;
   status2 = sciSetMarkStyle( pobj, (int) getDoubleFromStack(stackPointer) ) ;

  return sciSetFinalStatus( (SetPropertyStatus)status1, (SetPropertyStatus)status2 ) ;
}
/*------------------------------------------------------------------------*/
