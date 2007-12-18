/*------------------------------------------------------------------------*/
/* file: get_surface_color_property.c                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the surface_color field of       */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_surface_color_property( sciPointObj * pobj )
{
  sciSurface * ppSurface = NULL ;
  if ( sciGetEntityType (pobj) != SCI_SURFACE )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"surface_color") ;
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
