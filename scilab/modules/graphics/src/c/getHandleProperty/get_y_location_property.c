/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
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
/* file: get_y_location_property.c                                        */
/* desc : function to retrieve in Scilab the y_location field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_y_location_property( sciPointObj * pobj )
{
  int* location;

#if 0
  if (sciGetEntityType (pobj) != SCI_SUBWIN)
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"y_location");
    return -1;
  }
#endif

  location = (int*) getGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_LOCATION__, jni_int);

  if (location == NULL)
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"y_location");
    return -1;
  }

  if (*location == 4)
  {
    return sciReturnString( "left" );
  }
  else if (*location == 5)
  {
    return sciReturnString( "right" );
  }
  else if (*location == 2)
  {
    return sciReturnString( "middle" );
  }
  else if (*location == 3)
  {
    return sciReturnString( "origin" );
  }
  else
  {
    Scierror(999, _("Wrong value for '%s' property.\n"),"y_location");
    return -1;
  }

}
/*------------------------------------------------------------------------*/
