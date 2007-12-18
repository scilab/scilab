/*------------------------------------------------------------------------*/
/* file: get_data_bounds_property.c                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the data_bounds field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_data_bounds_property( sciPointObj * pobj )
{

  double bounds[6] ;
  sciGetDataBounds(pobj, bounds) ;

  if ( sciGetEntityType(pobj) == SCI_SUBWIN )
  {
    /**DJ.Abdemouche 2003**/
    if ( sciGetIs3d( pobj ) )
    {
      return sciReturnMatrix( bounds, 2, 3 ) ;
    }
    else
    {
      return sciReturnMatrix( bounds, 2, 2 ) ;
    }
  }
  else if ( sciGetEntityType (pobj) == SCI_SURFACE )
  {
    /* used for what ? F.Leray 20.04.05 */
    return sciReturnMatrix( bounds, 3, 2 ) ;
  }
  else
  {
    sciprint(_("%s property does not exist for this handle.\n"),"data_bounds");
    return -1;
  }

}
/*------------------------------------------------------------------------*/
