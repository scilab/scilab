/*------------------------------------------------------------------------*/
/* file: get_callbackmevent_property.c                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the callbackmevent field of      */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "Interaction.h"
#include "returnProperty.h"

/*------------------------------------------------------------------------*/
int get_callbackmevent_property( sciPointObj * pobj )
{
  return sciReturnInt( sciGetCallbackMouseEvent( pobj ) ) ;
}
/*------------------------------------------------------------------------*/
