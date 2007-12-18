/*------------------------------------------------------------------------*/
/* file: get_figure_size_property.c                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the figure_size field of a       */
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
int get_figure_size_property( sciPointObj * pobj )
{
  double figureSize[2] ;

  if ( sciGetEntityType (pobj) != SCI_FIGURE )
  {
    sciprint("figure_size property undefined for this object.\n") ;
    return -1;
  }

  figureSize[0] = sciGetWidth(pobj) ;
  figureSize[1] = sciGetHeight(pobj) ;

  return sciReturnRowVector( figureSize, 2 ) ;

}
/*------------------------------------------------------------------------*/
