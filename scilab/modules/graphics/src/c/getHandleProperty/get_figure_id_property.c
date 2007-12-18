/*------------------------------------------------------------------------*/
/* file: get_figure_id_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the figure_id field of a         */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_figure_id_property( sciPointObj * pobj )
{
  return sciReturnInt( sciGetNum( pobj ) ) ;
}
/*------------------------------------------------------------------------*/
