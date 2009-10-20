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
/* file: set_x_ticks_property.c                                           */
/* desc : function to modify in Scilab the x_ticks field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "CheckTicksProperty.h"
#include "MALLOC.h"
#include "BasicAlgos.h"
#include "DrawObjects.h"
#include "freeArrayOfString.h"
#include "loadTextRenderingAPI.h"

/*------------------------------------------------------------------------*/
int set_x_ticks_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  AssignedList * tlist     = NULL ;
  sciSubWindow * ppSubWin  = NULL ;
  int            nbTicsRow = 0    ;
  int            nbTicsCol = 0    ;
  char        ** labels    = NULL ; 

  if ( !isParameterTlist( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: Typed list expected.\n"), "x_ticks");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"x_ticks") ;
    return SET_PROPERTY_ERROR ;
  }

  ppSubWin = pSUBWIN_FEATURE(pobj) ;

  tlist = createTlistForTicks() ;

  if ( tlist == NULL )
  {
    return SET_PROPERTY_ERROR ;
  }

  /* locations */
  FREE( ppSubWin->axes.u_xgrads ) ;
  ppSubWin->axes.u_xgrads = NULL ;

  destroyStringArray( ppSubWin->axes.u_xlabels, ppSubWin->axes.u_nxgrads ) ;
  ppSubWin->axes.u_xlabels = NULL ;

  ppSubWin->axes.u_nxgrads = 0 ;

  ppSubWin->axes.u_xgrads = createCopyDoubleMatrixFromList( tlist, &nbTicsRow, &nbTicsCol ) ;

  if ( ppSubWin->axes.u_xgrads == NULL && nbTicsRow == -1 )
  {
    Scierror(999, _("%s: No more memory.\n"),"set_x_ticks_property");
    return SET_PROPERTY_ERROR ;
  }

  if ( ppSubWin->logflags[0] == 'l' )
  {
    int  i ;
    for ( i = 0 ; i < nbTicsCol * nbTicsRow ; i++ )
    {
      ppSubWin->axes.u_xgrads[i] = log10( ppSubWin->axes.u_xgrads[i] ) ;
    }
  }
  else
  {
     /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
    ppSubWin->axes.nbsubtics[0] = ComputeNbSubTics( pobj,ppSubWin->axes.u_nxgrads,'n',NULL,ppSubWin->axes.nbsubtics[0] ) ;
  }

  /*  labels */

  labels = getCurrentStringMatrixFromList( tlist, &nbTicsRow, &nbTicsCol );
  if( nbTicsCol * nbTicsRow )
  {

/* Check if we should load LaTex / MathML Java libraries */
	  loadTextRenderingAPI(labels, nbTicsCol, nbTicsRow);

    ppSubWin->axes.u_xlabels = createStringArrayCopy( labels,  nbTicsCol * nbTicsRow );
  }
  else
  {
    ppSubWin->axes.u_xlabels = NULL;
  }
  freeArrayOfString( labels, nbTicsRow * nbTicsCol );

  ppSubWin->axes.u_nxgrads = nbTicsRow * nbTicsCol ;
  ppSubWin->axes.auto_ticks[0] = FALSE ;

  destroyAssignedList( tlist ) ;

  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
