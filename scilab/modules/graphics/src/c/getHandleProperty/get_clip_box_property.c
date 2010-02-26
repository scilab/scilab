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
/* file: get_clip_box_property.c                                          */
/* desc : function to retrieve in Scilab the clip_box field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_clip_box_property( sciPointObj * pobj )
{
	int clipState = sciGetIsClipping ( pobj );
  if (clipState > 0)
  {
		/* clip state on */
    return sciReturnRowVector( sciGetClipping( pobj ), 4 ) ;
  }
  else if (clipState == 0 || clipState == -1)
	{
		/* clip state off or clipgrf */
    return sciReturnEmptyMatrix() ;
	}
	else
  { 
		/* error in retriveing clipping */
		Scierror(999, _("'%s' property does not exist for this handle.\n"),"clip_box");
		return -1;
  }
}
/*------------------------------------------------------------------------*/
