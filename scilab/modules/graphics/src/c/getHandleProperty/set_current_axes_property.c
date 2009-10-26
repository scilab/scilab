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
/* file: set_current_axes_property.c                                      */
/* desc : function to modify in Scilab the current_axes field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "HandleManagement.h"

/*------------------------------------------------------------------------*/
int set_current_axes_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  sciPointObj * curAxes   = NULL ;
  sciPointObj * parentFig = NULL ;
  int num = -1 ;
  int status1 = 1 ;
  int status2 = 1 ;
  

	if (pobj != NULL)
	{
		/* This property should not be called on an handle */
		Scierror(999, _("'%s' property does not exist for this handle.\n"), "current_axes");
		return SET_PROPERTY_ERROR;
	}

  if ( !isParameterHandle( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Handle expected.\n"), "current_axes");
    return SET_PROPERTY_ERROR ;
  }

  curAxes = sciGetPointerFromHandle( getHandleFromStack( stackPointer ) ) ;

  if ( curAxes == NULL)
  {
    Scierror(999, _("Wrong value for '%s' property: Must be a valid handle.\n"), "current_entity");
    return SET_PROPERTY_ERROR ;
  }
  if ( sciGetEntityType( curAxes ) != SCI_SUBWIN )
  {
    Scierror(999, _("Wrong value for '%s' property: Must be a handle on a axes.\n"), "current_axes");
    return SET_PROPERTY_ERROR ;
  }

  status1 = sciSetSelectedSubWin( curAxes ) ;
  /* F.Leray 11.02.05 : if the new selected subwin is not inside the current figure, */
  /* we must also set the current figure to subwin->parent */
  parentFig = sciGetParentFigure( curAxes );

  num  = sciGetNum( parentFig ) ;

  status2 = sciSetUsedWindow( num ) ;

  /* End modif. on the 11.02.05 */
  return sciSetFinalStatus( (SetPropertyStatus)status1, (SetPropertyStatus)status2 ) ;

}
/*------------------------------------------------------------------------*/
