/*------------------------------------------------------------------------*/
/* file: get_figure_style_property.c                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the figure_style field of a      */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "InitObjects.h"

/*------------------------------------------------------------------------*/
int get_figure_style_property( sciPointObj * pobj )
{
  /* no more old style */
  sciReturnString("new") ;
  return 0 ;
}
/*------------------------------------------------------------------------*/
