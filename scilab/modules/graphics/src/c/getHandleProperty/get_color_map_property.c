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
/* file: get_color_map_property.c                                         */
/* desc : function to retrieve in Scilab the color_map field of a         */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"


/*--------------------------------------------------------------------------*/
int get_color_map_property( sciPointObj * pobj )
{
  double * colorMap = NULL ;
  int cmapSize      = 0    ;
  int status        = -1   ;
  if ( sciGetEntityType( pobj ) != SCI_FIGURE )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"color_map");
    return -1;
  }

  cmapSize = sciGetNumColors(pobj) ;
  colorMap = MALLOC( cmapSize * 3 * sizeof(double) ) ;
  if ( colorMap == NULL )
  {
    Scierror(999, _("%s: No more memory.\n"),"get_color_map_property");
    return -1;
  }

  sciGetColormap(pobj, colorMap);

  status = sciReturnMatrix( colorMap, sciGetNumColors( pobj ), 3 ) ;

  FREE(colorMap) ;

  return status ;
}
/*--------------------------------------------------------------------------*/
