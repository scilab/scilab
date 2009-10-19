/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_figure_size_property.c                                       */
/* desc : function to modify in Scilab the figure_size field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"
#include "GraphicSynchronizerInterface.h"

/*------------------------------------------------------------------------*/
int set_figure_size_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  double * values = getDoubleMatrixFromStack( stackPointer ) ;
  int status;
  if ( sciGetEntityType(pobj) != SCI_FIGURE )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"figure_size");
    return SET_PROPERTY_ERROR ;
  }

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "figure_size");
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow * nbCol != 2 )
  {
    Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"),"figure_size",2) ;
    return SET_PROPERTY_ERROR ;
  }

  /* disable protection since this function will call Java */
  disableFigureSynchronization(pobj);
  status = sciSetWindowDim( pobj, (int)values[0], (int)values[1] ) ;
  enableFigureSynchronization(pobj);

  /* return set property unchanged since repaint is not really needed */
	return sciSetNoRedrawStatus((SetPropertyStatus)status);
}
/*------------------------------------------------------------------------*/
