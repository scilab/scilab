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
/* file: set_position_property.c                                          */
/* desc : function to modify in Scilab the position field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "SetUicontrolPosition.h"
/*------------------------------------------------------------------------*/
int set_position_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{

  if(sciGetEntityType(pobj) == SCI_UICONTROL || sciGetEntityType(pobj) == SCI_FIGURE)
  {
    SetUicontrolPosition(pobj, stackPointer, valueType, nbRow, nbCol);
    return SET_PROPERTY_SUCCEED ;
  }

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "position");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetAutoPosition( pobj ) )
  {
    sciSetAutoPosition( pobj, FALSE ) ;
  }

  if ( sciGetEntityType(pobj)== SCI_UIMENU )
  {
    pUIMENU_FEATURE(pobj)->MenuPosition = (int) getDoubleFromStack( stackPointer ) ;
    return SET_PROPERTY_SUCCEED ;
  }
  else if( sciGetEntityType(pobj) == SCI_LABEL )
  {
    double * values = getDoubleMatrixFromStack( stackPointer ) ;
    double curPos[3];
    sciGetTextPos(pobj, curPos);
    return sciSetTextPos( pobj, values[0], values[1], curPos[2] ) ;
  }
  else if ( sciGetEntityType(pobj) == SCI_LEGEND )
    {
      double * values = getDoubleMatrixFromStack( stackPointer ) ;
      return sciSetLegendPos( pobj, values ) ;
    }
  else
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"position");
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
