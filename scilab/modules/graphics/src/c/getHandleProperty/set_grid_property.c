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
/* file: set_grid_property.c                                              */
/* desc : function to modify in Scilab the grid field of                  */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_grid_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  int i ;
  int gridStyles[3];
  double * values = getDoubleMatrixFromStack( stackPointer ) ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "grid");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"grid") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow != 1 || nbCol > 3 )
  {
    Scierror(999, _("Wrong size for '%s' property: Must be in the set {%s}.\n"), "grid", "1x2, 1x3");
    return SET_PROPERTY_ERROR ;
  }

  sciGetGridStyle(pobj, &(gridStyles[0]), &(gridStyles[1]), &(gridStyles[2]));

  for (  i = 0 ; i < nbCol ; i++ )
  {
    int curValue = (int) values[i];
    if ( values[i] < -1 || !sciCheckColorIndex(pobj, curValue) )
    {
      Scierror(999, _("Wrong value for '%s' property: Must be -1 or a valid color index.\n"), "grid");
      return SET_PROPERTY_ERROR ;
    }
    gridStyles[i] = curValue ;
  }

  sciSetGridStyle(pobj, gridStyles[0], gridStyles[1], gridStyles[2]);
  

  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
