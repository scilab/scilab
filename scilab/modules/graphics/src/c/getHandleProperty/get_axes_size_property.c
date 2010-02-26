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
/* file: get_axes_size_property.c                                         */
/* desc : function to retrieve in Scilab the axes_size field of a         */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_axes_size_property( sciPointObj * pobj )
{
  double axesSize[2] ;

  if ( sciGetEntityType (pobj) != SCI_FIGURE )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"axes_size");
    return -1;
  }
  
  axesSize[0] = sciGetWidth(  pobj ) ; 
  axesSize[1] = sciGetHeight( pobj ) ; 

  return sciReturnRowVector( axesSize, 2 ) ;

}
/*------------------------------------------------------------------------*/
