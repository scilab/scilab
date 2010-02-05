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
/* file: set_figure_position_property.c                                   */
/* desc : function to modify in Scilab the screen_position field of       */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h" /* sciGetEntityType */
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "GraphicSynchronizerInterface.h"

/*------------------------------------------------------------------------*/
int set_screen_position_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  double * values;
  int status;
 
  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "screen_position");
    return SET_PROPERTY_ERROR ;
  }

	values = getDoubleMatrixFromStack( stackPointer ) ;

  if ( nbRow * nbCol != 2 )
  {
    Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "screen_position", 2);
    return SET_PROPERTY_ERROR ;
  }

	if ( sciGetEntityType(pobj) != SCI_FIGURE )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"screen_position");
    return SET_PROPERTY_ERROR ;
  }

  /* disable protection since this function will call Java */
  disableFigureSynchronization(pobj);
  status = sciSetScreenPosition( pobj, (int)values[0], (int)values[1]);
  enableFigureSynchronization(pobj);

  /* return set property unchanged since repaint is not really needed */
	return sciSetNoRedrawStatus((SetPropertyStatus)status);
}
/*------------------------------------------------------------------------*/
