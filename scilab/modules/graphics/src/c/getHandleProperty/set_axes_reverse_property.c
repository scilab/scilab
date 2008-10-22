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
/* file: set_axes_reverse_property.c                                      */
/* desc : function to modify in Scilab the axes_reverse field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_axes_reverse_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  char ** values = getStringMatrixFromStack( stackPointer ) ;

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"axes_reverse") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType( pobj ) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"axes_reverse") ;
    return SET_PROPERTY_ERROR ;
  }

  if( nbCol == 1 )
  {
    /* only one parameter to set the value for every axes.*/
    if ( strcmp( values[0], "off" ) == 0 ) 
    {
      pSUBWIN_FEATURE (pobj)->axes.reverse[0] = FALSE ;
      pSUBWIN_FEATURE (pobj)->axes.reverse[1] = FALSE ;
      pSUBWIN_FEATURE (pobj)->axes.reverse[2] = FALSE ;
    }
    else if ( strcmp( values[0], "on" ) == 0 )
    {
      pSUBWIN_FEATURE (pobj)->axes.reverse[0] = TRUE ;
      pSUBWIN_FEATURE (pobj)->axes.reverse[1] = TRUE ;
      pSUBWIN_FEATURE (pobj)->axes.reverse[2] = TRUE ;
    }
    else
    {
      sciprint(_("%s: Wrong input argument #%d: '%s' or '%s' expected."),"set_axes_reverse_property",2,"on","off") ;
      return SET_PROPERTY_ERROR ; ;
    }
    return SET_PROPERTY_SUCCEED ;
  }
  else if ( nbCol == 2 || nbCol == 3)
  {
    int i ;
    for ( i = 0; i < nbCol; i++ )
    {
      if ( strcmp(values[i],"off") == 0 )
      {
        pSUBWIN_FEATURE(pobj)->axes.reverse[i] = FALSE ;
      }
      else if ( strcmp(values[i],"on") == 0 )
      {
        pSUBWIN_FEATURE(pobj)->axes.reverse[i] = TRUE ;
      }
      else
      {
        sciprint(_("%s: Wrong input argument #%d: '%s' or '%s' expected."),"set_axes_reverse_property",2,"on","off");
        return SET_PROPERTY_ERROR ;
      }
    }
    return SET_PROPERTY_SUCCEED ;
  }
  else
  {
    sciprint(_("Wrong value for input argument #%d: Between %d to %d expected.\n"),2,1,3) ;
    return SET_PROPERTY_ERROR ;
  }
}
/*------------------------------------------------------------------------*/
