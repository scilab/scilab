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
/* file: set_auto_ticks_property.c                                        */
/* desc : function to modify in Scilab the auto_ticks field of            */
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
int set_auto_ticks_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  char ** values;

  if ( !isParameterStringMatrix( valueType ) )
  {
    Scierror(999, _("Incompatible type for property %s.\n"),"auto_ticks") ;
    return SET_PROPERTY_ERROR ;
  }

	values = getStringMatrixFromStack( stackPointer ) ;

  if ( sciGetEntityType( pobj ) != SCI_SUBWIN )
  {
    Scierror(999, _("%s property does not exist for this handle.\n"),"auto_ticks") ;
    return SET_PROPERTY_ERROR ;
  }

  if( nbCol == 1 )
  {
    /* only one parameter to set the value for every axes.*/
    if ( strcmp( values[0], "off" ) == 0 ) 
    {
      sciSetAutoTicks(pobj, FALSE, FALSE, FALSE);
    }
    else if ( strcmp( values[0], "on" ) == 0 )
    {
      sciSetAutoTicks(pobj, TRUE, TRUE, TRUE);
    }
    else
    {
      Scierror(999, _("%s: Wrong input argument #%d: '%s' or '%s' expected.\n"),"set_auto_ticks_property",2,"on","off") ;
      return SET_PROPERTY_ERROR ; ;
    }
    return SET_PROPERTY_SUCCEED ;
  }
  else if ( nbCol == 2 || nbCol == 3)
  {
    int i ;
    BOOL autoTicks[3];
    sciGetAutoTicks(pobj, autoTicks);
    for ( i = 0; i < nbCol; i++ )
    {
      if ( strcmp(values[i],"off") == 0 )
      {
        autoTicks[i] = FALSE ;
      }
      else if ( strcmp(values[i],"on") == 0 )
      {
        autoTicks[i] = TRUE ;
      }
      else
      {
        Scierror(999, _("%s: Wrong input argument #%d: '%s' or '%s' expected.\n"),"set_auto_ticks_property",2,"on","off");
        return SET_PROPERTY_ERROR ;
      }
    }
    sciSetAutoTicks(pobj, autoTicks[0], autoTicks[1], autoTicks[2]);
    return SET_PROPERTY_SUCCEED ;
  }
  else
  {
    Scierror(999, _("Wrong value for input argument #%d: Between %d to %d expected.\n"),2,1,3) ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
