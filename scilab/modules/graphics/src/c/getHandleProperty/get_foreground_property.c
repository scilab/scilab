/*------------------------------------------------------------------------*/
/* file: get_foregroud_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the foreground field of a        */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"

/*------------------------------------------------------------------------*/
int get_foreground_property( sciPointObj * pobj )
{
  return sciReturnDouble( sciGetForegroundToDisplay( pobj ) ) ;
}
/*------------------------------------------------------------------------*/
