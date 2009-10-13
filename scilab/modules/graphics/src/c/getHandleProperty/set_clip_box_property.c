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
/* file: set_clip_box_property.c                                          */
/* desc : function to modify in Scilab the clip_box field of              */
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
int set_clip_box_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  int status1 ;
  int status2 ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "clip_box");
    return SET_PROPERTY_ERROR ;
  }

  /* We must have 4 elements */
  if ( nbRow * nbCol != 4 )
  {
    Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "clip_box", 4);
    return SET_PROPERTY_ERROR ;
  }
  status1 = sciSetClipping( pobj, getDoubleMatrixFromStack( stackPointer ) ) ;
  status2 = sciSetIsClipping( pobj, 1 ) ;
  return sciSetFinalStatus( (SetPropertyStatus)status1, (SetPropertyStatus)status2 ) ;

}
/*------------------------------------------------------------------------*/
