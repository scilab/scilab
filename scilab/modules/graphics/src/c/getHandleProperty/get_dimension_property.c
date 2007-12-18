/*------------------------------------------------------------------------*/
/* file: get_size_property.c                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : return the dimension (width, height) in pixels of a graphical   */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_dimension_property( sciPointObj * pobj )
{
  int size[2] ;
  size[0] = sciGetWidth(  pobj ) ;
  size[1] = sciGetHeight( pobj ) ;
  return sciReturnRowVectorFromInt( size, 2 ) ;
}
/*------------------------------------------------------------------------*/
