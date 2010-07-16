/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

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

  cmapSize = *(int*)getGraphicObjectProperty(pobj->UID, __GO_COLORMAP_SIZE__, jni_int);

  colorMap = (double*)getGraphicObjectProperty(pobj->UID, __GO_COLORMAP__, jni_double_vector);

  status = sciReturnMatrix (colorMap, cmapSize/3, 3);

  return status;
}
/*--------------------------------------------------------------------------*/
