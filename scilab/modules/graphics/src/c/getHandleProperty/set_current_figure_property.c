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
#include "sciprint.h"
#include "localization.h"
#include "GraphicSynchronizerInterface.h"

/*------------------------------------------------------------------------*/
int set_current_figure_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  int figNum = -1 ;
  int res = -1 ;

  startGraphicDataReading();
  if ( isParameterHandle( valueType ) )
  {
    sciPointObj * curFig = sciGetPointerFromHandle( getHandleFromStack( stackPointer ) ) ;
    
    if ( curFig == NULL )
    {
      sciprint("Object is not valid.\n") ;
      endGraphicDataReading();
      return -1 ;
    }
    if ( sciGetEntityType( curFig ) != SCI_FIGURE )
    {
      sciprint("Object is not a handle on a figure.\n");
      endGraphicDataReading();
      return -1;
    }
    figNum = sciGetNum( curFig ) ;
  }
  else if ( isParameterDoubleMatrix( valueType ) )
  {
    figNum = (int) getDoubleFromStack( stackPointer ) ;
  }
  else
  {
    sciprint("Bad argument to determine the current figure: should be a window number or a handle (available under new graphics mode only).\n") ;
    endGraphicDataReading();
    return -1 ;
  }
  endGraphicDataReading();

  /* select the figure num */
  res = sciSetUsedWindow( figNum ) ;
  if ( res < 0 )
  {
    sciprint("It was not possible to create the requested figure.\n");
  }
  return res ;

}
/*------------------------------------------------------------------------*/
