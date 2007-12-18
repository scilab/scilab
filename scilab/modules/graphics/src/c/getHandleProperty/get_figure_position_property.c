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
    sciprint("figure_position property undefined for this object") ;
    return -1;
  }

  sciGetScreenPosition( pobj, &posX, &posY ) ;

  figurePos[0] = (double) posX ; 
  figurePos[1] = (double) posY ;

  
  return sciReturnRowVector( figurePos, 2 ) ;

}
/*------------------------------------------------------------------------*/
