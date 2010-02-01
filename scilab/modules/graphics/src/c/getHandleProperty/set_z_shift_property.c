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
/* file: set_z_shift_property.c                                           */
/* desc : function to modify in Scilab the z_shift field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "GetProperty.h"
#include "MALLOC.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_z_shift_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  int nbElement = nbRow * nbCol ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "z_shift");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_POLYLINE )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"z_shift");
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow > 1 && nbCol > 1 )
  {
    Scierror(999, _("Wrong size for '%s' property: Must be in the set {%s}.\n"), "z_shift", "0x0, 1xn, nx1");
    return SET_PROPERTY_ERROR ;
  }

  if ( nbElement != 0 && nbElement != pPOLYLINE_FEATURE (pobj)->n1 ) /* we can specify [] (null vector) to reset to default */
  {
    Scierror(999, _("Wrong size for '%s' property: %d or %d elements expected.\n"), "z_shift", 0, pPOLYLINE_FEATURE (pobj)->n1);
    return SET_PROPERTY_ERROR ;
  }

  FREE( pPOLYLINE_FEATURE (pobj)->z_shift ) ;
  pPOLYLINE_FEATURE(pobj)->z_shift = NULL;

  if( nbElement != 0 )
  {
    pPOLYLINE_FEATURE(pobj)->y_shift = createCopyDoubleVectorFromStack( stackPointer, nbElement ) ;

    if ( pPOLYLINE_FEATURE (pobj)->y_shift == NULL )
    {
      Scierror(999, _("%s: No more memory.\n"),"set_z_shift_property");
      return SET_PROPERTY_ERROR ;
    }
  }
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
