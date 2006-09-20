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
#include "Format.h"
#include "get_ticks_utils.h"
#include "MALLOC.h"

/*------------------------------------------------------------------------*/
int get_x_ticks_property( sciPointObj * pobj )
{
  sciSubWindow * ppSubWin = NULL ;
  char c_format[5];
  if ( sciGetEntityType( pobj ) != SCI_SUBWIN )
  {
    sciprint("x_ticks property does not exists for this label.\n") ;
    return -1 ;
  }

  ppSubWin = pSUBWIN_FEATURE( pobj ) ;

  /* compute the c_format used for convert double to char (for labels) */
  ChooseGoodFormat( c_format, ppSubWin->logflags[0], ppSubWin->axes.xgrads, ppSubWin->axes.nxgrads ) ;

  if( ppSubWin->axes.auto_ticks[0] )
  {
    int       nbtics        = ppSubWin->axes.nxgrads ;
    char   ** ticklabel     = NULL                   ;
    double *  ticksPosition = NULL                   ;
    
    ticksPosition = ReBuildTicksLog2Lin( ppSubWin->logflags[0], nbtics, ppSubWin->axes.xgrads ) ;
    
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
    int      nbtics        = ppSubWin->axes.u_nxgrads ;
    double * ticksPosition = ReBuildTicksLog2Lin(ppSubWin->logflags[0],nbtics,ppSubWin->axes.u_xgrads) ;
    
    buildTListForTicks( ticksPosition, ppSubWin->axes.u_xlabels, nbtics ) ;
    
    FREE( ticksPosition ) ;
    return 0 ;
  }

  return -1 ;
}
/*------------------------------------------------------------------------*/
