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
/* file: get_data_bounds_property.c                                       */
/* desc : function to retrieve in Scilab the data_bounds field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_data_bounds_property( sciPointObj * pobj )
{

  if ( sciGetEntityType(pobj) == SCI_SUBWIN )
  {
		double bounds[6] ;
		sciGetDataBounds(pobj, bounds) ;
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
		double bounds[6] ;
		sciGetDataBounds(pobj, bounds) ;
    /* used for what ? F.Leray 20.04.05 */
    return sciReturnMatrix( bounds, 3, 2 ) ;
  }
  else
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"data_bounds");
    return -1;
  }

}
/*------------------------------------------------------------------------*/
