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
/* file: get_figure_position_property.c                                   */
/* desc : function to retrieve in Scilab the figure_position field of a   */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"
#include "InitObjects.h"

/*------------------------------------------------------------------------*/
int get_figure_position_property( sciPointObj * pobj )
{
  int posX ;
  int posY ;
  double figurePos[2] ;

  if ( sciGetEntityType (pobj) != SCI_FIGURE )
  {
    sciprint(_("%s undefined for this object.\n"), "figure_position") ;
    return -1;
  }

  sciGetScreenPosition( pobj, &posX, &posY ) ;

  figurePos[0] = (double) posX ; 
  figurePos[1] = (double) posY ;

  
  return sciReturnRowVector( figurePos, 2 ) ;

}
/*------------------------------------------------------------------------*/
