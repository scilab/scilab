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
/* file: get_figures_id_property.c                                        */
/* desc : function to retrieve in Scilab the figures_id field of a handle */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "WindowList.h"

/*------------------------------------------------------------------------*/
int get_figures_id_property( sciPointObj * pobj )
{
  int   nbFig  = 0    ;
  int * ids    = NULL ;
  int   status = -1   ;

	if (pobj != NULL)
	{
		/* This property should not be called on an handle */
		Scierror(999, _("'%s' property does not exist for this handle.\n"), "figures_id");
		return -1;
	}

  nbFig = sciGetNbFigure() ; /* get the number of opened windows */
  
  ids = MALLOC( nbFig * sizeof(int) ) ;
  if ( ids == NULL )
  { 
	  Scierror(999, _("%s: No more memory.\n"),"get_figures_id_property");
	  return -1 ;
  }

  sciGetFiguresId( ids ) ;

  status = sciReturnRowIntVector( ids, nbFig ) ;

  FREE( ids ) ;

  return status ;

}
/*------------------------------------------------------------------------*/
