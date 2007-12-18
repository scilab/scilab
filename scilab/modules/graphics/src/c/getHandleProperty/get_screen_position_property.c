/*------------------------------------------------------------------------*/
/* file: get_screen_position_property.c                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the position of an object in     */
/*        the screen                                                      */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_screen_position_property( sciPointObj * pobj )
{

  int pos[2] ;
  sciGetScreenPosition( pobj, &pos[0], &pos[1] ) ;
  return sciReturnRowVectorFromInt( pos, 2 ) ;
}
/*------------------------------------------------------------------------*/
