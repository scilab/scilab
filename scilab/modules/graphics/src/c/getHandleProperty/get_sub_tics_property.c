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
/* file: get_sub_tics_property.c                                          */
/* desc : function to retrieve in Scilab the sub_tics or sub_ticks field  */
/*        of a handle                                                     */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_sub_tics_property( sciPointObj * pobj )
{

  /*Dj.A 17/12/2003*/
  /* modified jb Silvy 01/2006 */

  if ( sciGetEntityType (pobj) == SCI_AXES )
  {
    return sciReturnDouble( pAXES_FEATURE(pobj)->subint ) ;
  }
  else if ( sciGetEntityType (pobj) == SCI_SUBWIN )
  {
    double sub_ticks[3] ;
    int i ;
    for ( i = 0 ; i < 3 ; i++ )
    {
      sub_ticks[i] = pSUBWIN_FEATURE (pobj)->axes.nbsubtics[i];
    }
    if ( sciGetIs3d( pobj ) )
    {
      return sciReturnRowVector( sub_ticks, 3 ) ;
    }
    else
    {
      return sciReturnRowVector( sub_ticks, 2 ) ;
    }
  }
  else
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"sub_ticks") ;
    return -1 ;
  }
}
/*------------------------------------------------------------------------*/
