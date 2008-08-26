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
/* file: set_margins_property.c                                           */
/* desc : function to modify in Scilab the margins field of               */
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
int set_margins_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"margins") ;
    return SET_PROPERTY_ERROR ;
  }

  if (sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"margins") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow * nbCol != 4 )
  {
    sciprint("Second argument must have 4 elements.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  copyDoubleVectorFromStack( stackPointer, pSUBWIN_FEATURE (pobj)->ARect, 4 ) ;

  return SET_PROPERTY_SUCCEED ;
  
}
/*------------------------------------------------------------------------*/
