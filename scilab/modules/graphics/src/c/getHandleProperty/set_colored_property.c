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
/* file: set_colored_property.c                                           */
/* desc : function to modify in Scilab the colored field of               */
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
int set_colored_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  
  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"colored") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType( pobj ) != SCI_SEGS || pSEGS_FEATURE(pobj)->ptype == 0 )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"colored") ;
    return SET_PROPERTY_ERROR ;
  }

    if ( isStringParamEqual( stackPointer, "on") )
    {
      pSEGS_FEATURE(pobj)->typeofchamp = 1 ;
    }
    else if ( isStringParamEqual( stackPointer, "off") )
    {
      pSEGS_FEATURE(pobj)->typeofchamp = 0 ;
    }
    else
    {
      sciprint("Value must be 'on' or 'off'.\n") ;
      return SET_PROPERTY_ERROR ;
    }

    return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
