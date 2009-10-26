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
/* file: get_y_ticks_property.c                                           */
/* desc : function to retrieve in Scilab the y_ticks field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "get_ticks_utils.h"
#include "MALLOC.h"
#include "BasicAlgos.h"

/*------------------------------------------------------------------------*/
int get_y_ticks_property( sciPointObj * pobj )
{
  
  int nbTicks;

  if ( sciGetEntityType( pobj ) != SCI_SUBWIN )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"y_ticks");
    return -1 ;
  }

  /* retrieve number of ticks */
  nbTicks = sciGetNbYTicks(pobj);
  if (nbTicks == 0)
  {
    /* return empty matrices */
    buildTListForTicks( NULL, NULL, 0) ;
  }
  else
  {
    char ** labels;
    double * positions;
    /* allocate arrays */
    positions = MALLOC(nbTicks * sizeof(double));
    labels = createStringArray(nbTicks);

    sciGetYTicksPos(pobj, positions, labels);

    buildTListForTicks( positions, labels, nbTicks ) ;

    /* free arrays */
    destroyStringArray(labels, nbTicks);
    FREE(positions);
  }

  return 0;

}
/*------------------------------------------------------------------------*/
