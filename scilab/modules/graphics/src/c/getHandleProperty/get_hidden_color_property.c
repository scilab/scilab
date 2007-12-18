/*------------------------------------------------------------------------*/
/* file: get_hidden_color_property.c                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the hidden_color field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_hidden_color_property( sciPointObj * pobj )
{
  return sciReturnDouble( sciGetHiddenColor( pobj ) ) ;
}
/*------------------------------------------------------------------------*/
