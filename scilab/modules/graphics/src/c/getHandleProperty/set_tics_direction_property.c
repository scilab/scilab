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
/* file: set_tics_direction_property.c                                    */
/* desc : function to modify in Scilab the tics_direction field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_tics_direction_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"tics_direction") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_AXES )
  {
    sciprint("tics_direction property does not exist for this handle.\n" ) ;
    return SET_PROPERTY_ERROR ;
  }

  if ( pAXES_FEATURE (pobj)->ny == 1 )
  { 
    if( isStringParamEqual( stackPointer, "top" ) )
    {
      pAXES_FEATURE (pobj)->dir = 'u' ;
    }
    else if ( isStringParamEqual( stackPointer, "bottom" ) )
    {
      pAXES_FEATURE (pobj)->dir = 'd' ;
    }
    else
    {
      sciprint(_("%s: Wrong type for second input argument: '%s' or '%s' expected.\n"), "set_tics_direction_property","top","bottom") ;
      return SET_PROPERTY_ERROR ;
    }
    return SET_PROPERTY_SUCCEED ;
  } 
  else
  {
    if( isStringParamEqual( stackPointer, "right" ) )
    {
      pAXES_FEATURE (pobj)->dir = 'r' ;
    }
    else if ( isStringParamEqual( stackPointer, "left" ) )
    {
      pAXES_FEATURE (pobj)->dir = 'l' ;
    }
    else
    {
      sciprint(_("%s: Wrong type for second input argument: '%s' or '%s' expected.\n"), "set_tics_direction_property","right","left") ;
      return -1 ;
    }
    return SET_PROPERTY_SUCCEED ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
