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
/* file: get_surface_color_property.c                                     */
/* desc : function to retrieve in Scilab the surface_color field of       */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
/*------------------------------------------------------------------------*/
int get_surface_color_property( sciPointObj * pobj )
{
  sciSurface * ppSurface = NULL ;
  if ( sciGetEntityType (pobj) != SCI_SURFACE )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"surface_color") ;
    return -1 ;
  }

  ppSurface = pSURFACE_FEATURE (pobj) ;

  if ( ppSurface->typeof3d == SCI_PARAM3D1 )
  {
    return sciReturnRowVector( ppSurface->zcol, ppSurface->dimzy ) ;
  }
  else if ( ppSurface->typeof3d == SCI_FAC3D)
  {
    if ( ppSurface->flagcolor == 2 || ppSurface->flagcolor == 4 )
    {
      return sciReturnRowVector( ppSurface->zcol, ppSurface->dimzy ) ;
    }
    else if ( ppSurface->flagcolor == 3 )
    {
     return sciReturnMatrix( ppSurface->zcol, ppSurface->dimzx, ppSurface->dimzy ) ;
    }
    else
    {
     return sciReturnEmptyMatrix() ;
    }

  }

  return -1 ;

}
/*------------------------------------------------------------------------*/
