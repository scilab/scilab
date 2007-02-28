/*------------------------------------------------------------------------*/
/* file: get_figure_position_property.c                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the figure_position field of a   */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "InitObjects.h"

/*------------------------------------------------------------------------*/
int get_figure_position_property( sciPointObj * pobj )
{
  double figurePos[2] ;

  if ( sciGetEntityType (pobj) != SCI_FIGURE )
  {
    sciprint("figure_position property undefined for this object" ) ;
    return -1;
  }

  if ( pobj != getFigureModel())
  {
    int posX ;
    int posY ;
    sciGetScreenPosition( pobj, &posX, &posY ) ;
#if _MSC_VER /* Correction pour figure_position (Windows) Allan CORNET Mai 2004 */
    figurePos[0] = (double) posX ; 
    figurePos[1] = (double) posY ;
#else
    figurePos[0] = (double) posX - 4 ; 
    figurePos[1] = (double) posY - 20 ;
#endif
  }
  else
  {
    figurePos[0] = pFIGURE_FEATURE (pobj)->inrootposx; 
    figurePos[1] = pFIGURE_FEATURE (pobj)->inrootposy;
  }
  
  return sciReturnRowVector( figurePos, 2 ) ;

}
/*------------------------------------------------------------------------*/
