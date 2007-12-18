/*------------------------------------------------------------------------*/
/* file: get_default_figure_property.c                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the default_figure field of a    */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"
#include "InitObjects.h"

/*--------------------------------------------------------------------------*/
int get_default_figure_property( sciPointObj * pobj )
{
  if ( getFigureModel() !=  NULL )
  {
    return sciReturnHandle( sciGetHandle(getFigureModel()) ) ;
  }
  else
  {
    sciprint("Default figure does not exist.\n");
    return -1;
  }
}
/*--------------------------------------------------------------------------*/
