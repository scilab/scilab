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
/* file: get_position_property.c                                          */
/* desc : function to retrieve in Scilab the position field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_position_property( sciPointObj * pobj )
{
  

  if (sciGetEntityType(pobj) == SCI_UIMENU )
    {
      return sciReturnDouble( pUIMENU_FEATURE(pobj)->MenuPosition ) ;
    }
  else if (sciGetEntityType(pobj) == SCI_UICONTROL)
    {
      return  GetUicontrolPosition(pobj);
    }
  else if (sciGetEntityType(pobj) == SCI_FIGURE) // Uicontrol figure
    {
      double position[4];
      int posX = 0, posY = 0;

      sciGetScreenPosition(pobj, &posX, &posY) ;
      position[0] = (double) posX; 
      position[1] = (double) posY;
      position[2] = sciGetWindowWidth(pobj);
      position[3] = sciGetWindowHeight(pobj);
      
      return sciReturnRowVector(position, 4) ;
    }
  else if ( sciGetEntityType(pobj) == SCI_LABEL )
    {
      double position[3] ;
      sciGetTextPos( pobj, position ) ;
      return sciReturnRowVector( position, 2 ) ;
    }
  else if ( sciGetEntityType(pobj) == SCI_LEGEND )
    {
      double position[2] ;
      sciGetLegendPos( pobj, position ) ;
      return sciReturnRowVector( position, 2 ) ;
    }
  Scierror(999, _("'%s' property does not exist for this handle.\n"), "position") ;
  return -1;
}
/*------------------------------------------------------------------------*/ 
