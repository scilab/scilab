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
/* file: set_figure_name_property.c                                       */
/* desc : function to modify in Scilab the figure_name field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "GraphicSynchronizerInterface.h"

/*------------------------------------------------------------------------*/
int set_figure_name_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  int status;
  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"figure_name") ;
    return SET_PROPERTY_ERROR ;
  }
  /* disable protection since this function will call Java */
  disableFigureSynchronization(pobj);
  status = sciSetName( pobj, getStringFromStack( stackPointer )) ;
  enableFigureSynchronization(pobj);

  return status;
}
/*------------------------------------------------------------------------*/
