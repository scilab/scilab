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
/* file: set_surface_color_property.c                                     */
/* desc : function to modify in Scilab the surface_color field of         */
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
int set_surface_color_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "surface_color");
    return SET_PROPERTY_ERROR ;
  }

  if (sciGetEntityType (pobj) != SCI_SURFACE)
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"surface_color") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( pSURFACE_FEATURE(pobj)->typeof3d == SCI_PARAM3D1 )
  {
    if (pSURFACE_FEATURE (pobj)->dimzy != nbRow * nbCol )
    {
      Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "surface_color", pSURFACE_FEATURE (pobj)->dimzy);
      return SET_PROPERTY_ERROR ;
    }
    copyDoubleVectorFromStack( stackPointer, pSURFACE_FEATURE(pobj)->zcol, pSURFACE_FEATURE (pobj)->dimzy ) ;
  }
  else if ( pSURFACE_FEATURE (pobj)->typeof3d == SCI_FAC3D && pSURFACE_FEATURE (pobj)->flagcolor >= 2 )
  {
    int N;
    if ( pSURFACE_FEATURE(pobj)->flagcolor == 2 )
    {
      N = pSURFACE_FEATURE (pobj)->dimzy ;
    }
    else
    {
      N = pSURFACE_FEATURE(pobj)->dimzy * pSURFACE_FEATURE (pobj)->dimzx;
    }
    if ( nbRow * nbCol != N)
    {
      Scierror(999, _("Argument #%d must have %d elements.\n"),2,N) ;
      Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "surface_color", N);
      return SET_PROPERTY_ERROR ;
    }
    copyDoubleVectorFromStack( stackPointer, pSURFACE_FEATURE (pobj)->zcol, N ) ;
  }
  else
  {
    Scierror(999, _("%s cannot be set in this case.\n"),"'surface_color'");
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
