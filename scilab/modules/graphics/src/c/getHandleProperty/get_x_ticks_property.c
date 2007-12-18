/*------------------------------------------------------------------------*/
/* file: get_x_ticks_property.c                                           */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the x_ticks field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
#include "Format.h"
#include "get_ticks_utils.h"
#include "MALLOC.h"
#include "BasicAlgos.h"

/*------------------------------------------------------------------------*/
int get_x_ticks_property( sciPointObj * pobj )
{
  sciSubWindow * ppSubWin = NULL ;
  BOOL autoTicks[3];
  char c_format[5];
  if ( sciGetEntityType( pobj ) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this label.\n"),"x_ticks") ;
    return -1 ;
  }

  ppSubWin = pSUBWIN_FEATURE( pobj ) ;

  /* compute the c_format used for convert double to char (for labels) */
  ChooseGoodFormat( c_format, ppSubWin->logflags[0], ppSubWin->axes.xgrads, ppSubWin->axes.nxgrads ) ;

  sciGetAutoTicks(pobj, autoTicks);
  if( autoTicks[0] )
  {
    int       nbtics        = ppSubWin->axes.nxgrads ;
    char   ** tickslabel    = NULL                   ;
    double *  ticksPosition = NULL                   ;
    
    ticksPosition = ReBuildTicksLog2Lin( ppSubWin->logflags[0], nbtics, ppSubWin->axes.xgrads ) ;
    
    /* convert double to strings */
    tickslabel = copyFormatedArray( ticksPosition, nbtics, c_format, 100 ) ;
    
    /* construction de la tlist */
    buildTListForTicks( ticksPosition, tickslabel, nbtics ) ;
    
    /* free tickslabel */
    destroyStringArray( tickslabel, nbtics ) ;
    FREE( ticksPosition ) ;
    return 0 ;
  }
  else /* we display the x tics specified by the user*/
  {
    int      nbtics        = ppSubWin->axes.u_nxgrads ;
    double * ticksPosition = ReBuildTicksLog2Lin(ppSubWin->logflags[0],nbtics,ppSubWin->axes.u_xgrads) ;
    
    buildTListForTicks( ticksPosition, ppSubWin->axes.u_xlabels, nbtics ) ;
    
    FREE( ticksPosition ) ;
    return 0 ;
  }

}
/*------------------------------------------------------------------------*/
