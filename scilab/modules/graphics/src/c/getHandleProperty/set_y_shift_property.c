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
/* file: set_y_shift_property.c                                           */
/* desc : function to modify in Scilab the y_shift field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "GetProperty.h"
#include "MALLOC.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_y_shift_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  int nbElement = nbRow * nbCol ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"y_shift") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_POLYLINE )
  {
    sciprint("This handle has no y_shift property.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow > 1 && nbCol > 1 )
  {
    sciprint("Bad input, y_shift should be a row or column vector.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbElement != 0 && nbElement != pPOLYLINE_FEATURE (pobj)->n1 ) /* we can specify [] (null vector) to reset to default */
  {
    sciprint("Wrong size for input vector.\n");
    return SET_PROPERTY_ERROR ;
  }

  FREE( pPOLYLINE_FEATURE(pobj)->y_shift ) ;
  pPOLYLINE_FEATURE(pobj)->y_shift = NULL;

  if( nbElement != 0 )
  {
    pPOLYLINE_FEATURE(pobj)->y_shift = createCopyDoubleVectorFromStack( stackPointer, nbElement ) ;

    if ( pPOLYLINE_FEATURE (pobj)->y_shift == NULL )
    {
      sciprint(_("%s: No more memory.\n"),"set_y_shift_property");
      return SET_PROPERTY_ERROR ;
    }
  }
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
