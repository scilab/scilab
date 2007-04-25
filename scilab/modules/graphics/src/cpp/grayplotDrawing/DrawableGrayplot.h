/*------------------------------------------------------------------------*/
/* file: DrawableGrayplot.h                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a grayplot object      */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_GRAYPLOT_H_
#define _DRAWABLE_GRAYPLOT_H_


#include "../DrawableClippedObject.h"

namespace sciGraphics
{

class DrawableGrayplot : public DrawableClippedObject
{

public:

  DrawableGrayplot( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}

  /**
   * Create a new drawer from the drawed object.
   */
  void draw( void ) ;

} ;

}

#endif /* _DRAWABLE_GRAYPLOT_H_  */
