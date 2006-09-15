/*------------------------------------------------------------------------*/
/* file: get_clip_box_property.c                                          */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the clip_box field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"

/*------------------------------------------------------------------------*/
int get_clip_box_property( sciPointObj * pobj )
{
  if ( sciGetIsClipping ( pobj ) > 0 )
  {
    return sciReturnRowVector( sciGetClipping( pobj ), 4 ) ;
  }
  else
  { 
    return sciReturnEmptyMatrix() ;
  }
  return -1 ;
}
/*------------------------------------------------------------------------*/
