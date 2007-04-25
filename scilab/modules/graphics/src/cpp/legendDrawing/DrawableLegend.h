/*------------------------------------------------------------------------*/
/* file: DrawableLegend.h                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a legend object        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_LEGEND_H_
#define _DRAWABLE_LEGEND_H_


#include "../DrawableObject.h"

class DrawableLegend : public DrawableObject
{

public:

  DrawableLegend( sciPointObj * pObj ) : DrawableObject( pObj ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  void draw( void ) ;

} ;

#endif /* _DRAWABLE_LEGEND_H_  */
