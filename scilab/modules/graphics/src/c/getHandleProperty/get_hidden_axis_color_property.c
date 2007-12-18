/*------------------------------------------------------------------------*/
/* file: get_hidden_axis_color_property.c                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the hidden_axis_color field of   */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_hidden_axis_color_property( sciPointObj * pobj )
{
  if ( sciGetEntityType (pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"hidden_axis_color");
    return -1;
  }
  return sciReturnInt( sciGetHiddenAxisColor(pobj)) ;
}
/*------------------------------------------------------------------------*/
