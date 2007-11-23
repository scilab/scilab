/*------------------------------------------------------------------------*/
/* file: get_default_axes_property.c                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the default_axes field of a      */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "InitObjects.h"

/*--------------------------------------------------------------------------*/
int get_default_axes_property( sciPointObj * pobj )
{
  if ( getAxesModel() != NULL ) 
  {
    return sciReturnHandle( sciGetHandle( getAxesModel() ) ) ;
  }
  else
  {
    sciprint("Default axes do not exist.");
    return -1;
  }
}
/*--------------------------------------------------------------------------*/
