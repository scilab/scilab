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
/* file: set_data_mapping_property.c                                      */
/* desc : function to modify in Scilab the data_mapping field of          */
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
int set_data_mapping_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"data_mapping") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_GRAYPLOT )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"data_mapping") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "scaled") || isStringParamEqual( stackPointer, "direct") )
  {
     strcpy( pGRAYPLOT_FEATURE (pobj)->datamapping, getStringFromStack(stackPointer) ) ;
     return SET_PROPERTY_SUCCEED ;
  }
  else
  {
    sciprint("Value must be 'direct' or 'scaled'.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_ERROR ;


}
/*------------------------------------------------------------------------*/
