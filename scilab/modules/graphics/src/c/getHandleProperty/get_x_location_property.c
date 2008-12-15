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
/* file: get_x_location_property.c                                        */
/* desc : function to retrieve in Scilab the x_location field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_x_location_property( sciPointObj * pobj )
{
  if (sciGetEntityType (pobj) != SCI_SUBWIN)
  {
    Scierror(999, _("%s property undefined for this handle.\n"), "x_location") ;
    return -1 ;
  }

  switch ( pSUBWIN_FEATURE (pobj)->axes.xdir )
  {
  case 'u': 
    return sciReturnString( "top" ) ; 
    break;
  case 'd': 
    return sciReturnString( "bottom" ) ;
    break;
  case 'c': 
    return sciReturnString( "origin" ) ;
    break;
  default : 
    Scierror(999, _("%s is not correctly defined.\n"), "x_location") ;
    break;
  }
  return -1 ;
}
/*------------------------------------------------------------------------*/
