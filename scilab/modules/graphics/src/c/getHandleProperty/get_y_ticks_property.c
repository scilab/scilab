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
#include "sciprint.h"
#include "localization.h"
#include "get_ticks_utils.h"
#include "Format.h"
#include "MALLOC.h"
#include "BasicAlgos.h"

/*------------------------------------------------------------------------*/
int get_y_ticks_property( sciPointObj * pobj )
{
  sciSubWindow * ppSubWin = NULL ;
  BOOL autoTicks[3];
  char c_format[5];
  if ( sciGetEntityType( pobj ) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this label.\n"),"y_ticks") ;
    return -1 ;
  }

  ppSubWin = pSUBWIN_FEATURE( pobj ) ;

  /* compute the c_format used for convert double to char (for labels) */
  ChooseGoodFormat( c_format, ppSubWin->logflags[1], ppSubWin->axes.ygrads, ppSubWin->axes.nygrads ) ;

  sciGetAutoTicks(pobj, autoTicks);
  if( autoTicks[1] )
  {
    int       nbtics        = ppSubWin->axes.nygrads ;
    char   ** ticklabel     = NULL                   ;
    double *  ticksPosition = NULL                   ;

    ticksPosition = ReBuildTicksLog2Lin( ppSubWin->logflags[1], nbtics, ppSubWin->axes.ygrads ) ;

    /* convert double to strings */
    ticklabel = copyFormatedArray( ticksPosition, nbtics, c_format, 100 ) ;

    /* construction de la tlist */
    buildTListForTicks( ticksPosition, ticklabel, nbtics ) ;

    /* free ticklabel */
    destroyStringArray( ticklabel, nbtics ) ;
    FREE( ticksPosition ) ;
    return 0 ;
  }
  else /* we display the x tics specified by the user*/
  {
    int      nbtics        = ppSubWin->axes.u_nygrads ;
    double * ticksPosition = ReBuildTicksLog2Lin( ppSubWin->logflags[1], nbtics, ppSubWin->axes.u_ygrads ) ;

    buildTListForTicks( ticksPosition, ppSubWin->axes.u_ylabels, nbtics ) ;

    FREE( ticksPosition ) ;
    return 0 ;
  }

}
/*------------------------------------------------------------------------*/
