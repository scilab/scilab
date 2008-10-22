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
/* file: set_current_entity_property.c                                    */
/* desc : function to modify in Scilab the current_entity field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "CurrentObjectsManagement.h"

/*------------------------------------------------------------------------*/
int set_current_entity_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  sciPointObj * curEntity = NULL ;
  
  if ( !isParameterHandle( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"current_entity") ;
    return SET_PROPERTY_ERROR ;
  }

  curEntity = sciGetPointerFromHandle( getHandleFromStack( stackPointer ) ) ;

  if ( curEntity == NULL )
  {
    sciprint("Object is not valid.\n") ;
    return SET_PROPERTY_ERROR ;
  }
  sciSetCurrentObj( curEntity ) ;
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
