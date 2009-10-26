/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_axes_visible_property.c                                      */
/* desc : function to modify in Scilab the grid field of                  */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_axes_visible_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  char ** values = getStringMatrixFromStack( stackPointer ) ;

  if ( !isParameterStringMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: String matrix expected.\n"), "axes_visible");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"axes_visible") ;
    return SET_PROPERTY_ERROR ;
  }

  if( nbCol == 1 )
  {

    if ( strcmp( values[0], "off") == 0 )
    {
      pSUBWIN_FEATURE (pobj)->axes.axes_visible[0] = FALSE ;
      pSUBWIN_FEATURE (pobj)->axes.axes_visible[1] = FALSE ;
      pSUBWIN_FEATURE (pobj)->axes.axes_visible[2] = FALSE ;
    }
    else if ( strcmp( values[0], "on") == 0 )
    {
      pSUBWIN_FEATURE(pobj)->axes.axes_visible[0] = TRUE ;
      pSUBWIN_FEATURE(pobj)->axes.axes_visible[1] = TRUE ;
      pSUBWIN_FEATURE(pobj)->axes.axes_visible[2] = TRUE ;
    }
    else
    {
      Scierror(999, _("Wrong value for '%s' property: %s or %s expected.\n"), "axes_visible", "on", "off");
      return SET_PROPERTY_ERROR ;
    }
    return SET_PROPERTY_SUCCEED ;
  }
  else if ( nbCol == 2 || nbCol == 3 )
  {
    int i ;
    for ( i = 0; i < nbCol ; i++ )
    {
      if ( strcmp( values[i], "off" ) == 0)
      {
        pSUBWIN_FEATURE (pobj)->axes.axes_visible[i] = FALSE ;
      }
      else if ( strcmp( values[i], "on" ) == 0 )
      {
        pSUBWIN_FEATURE (pobj)->axes.axes_visible[i] = TRUE ;
      }
      else
      {
        Scierror(999, _("Wrong value for '%s' property: %s or %s expected.\n"), "axes_visible", "on", "off");
        return SET_PROPERTY_ERROR ;
      }
    }
    return SET_PROPERTY_SUCCEED ;
  }
  else
  {
    Scierror(999, _("Wrong size for '%s' property: At most %d elements expected.\n"), "axes_visible", 3);
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
