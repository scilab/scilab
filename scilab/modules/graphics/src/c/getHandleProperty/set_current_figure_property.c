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
/* file: set_current_figure_property.c                                    */
/* desc : function to modify in Scilab the current_figure field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "GraphicSynchronizerInterface.h"

/*------------------------------------------------------------------------*/
int set_current_figure_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  int figNum = -1 ;
  int res = -1 ;


	if (pobj != NULL)
	{
		/* This property should not be called on an handle */
		Scierror(999, _("%s property does not exist for this handle.\n"), "current_figure");
		return -1;
	}

  if (nbRow * nbCol != 1)
  {
    Scierror(999, _("Wrong size for input argument #%d: A real or a 'Figure' handle expected.\n"), 1) ;
    return -1 ;
  }
  
  if ( isParameterHandle( valueType ) )
  {

    sciPointObj * curFig = sciGetPointerFromHandle( getHandleFromStack( stackPointer ) ) ;
    
    if ( curFig == NULL )
    {
      Scierror(999, _("'%s' handle does not or no longer exists.\n"),"Figure");
      return -1 ;
    }

    if ( sciGetEntityType( curFig ) != SCI_FIGURE )
    {
      Scierror(999, _("Wrong type for input argument #%d: A real or a '%s' handle expected.\n"), 1,"Figure") ;
      return -1;
    }
    startGraphicDataReading();
    figNum = sciGetNum( curFig ) ;
    endGraphicDataReading();
  }
  else if ( isParameterDoubleMatrix( valueType ) )
  {
    figNum = (int) getDoubleFromStack( stackPointer ) ;
  }
  else
  {
    Scierror(999, _("Wrong type for input argument #%d: A real or a '%s' handle expected.\n"), 1,"Figure") ;
    return -1 ;
  }

  /* select the figure num */
  res = sciSetUsedWindow( figNum ) ;
  if ( res < 0 )
  {
    Scierror(999, _("Unable to create requested figure: No more memory.\n"));
  }
  return res ;

}
/*------------------------------------------------------------------------*/
