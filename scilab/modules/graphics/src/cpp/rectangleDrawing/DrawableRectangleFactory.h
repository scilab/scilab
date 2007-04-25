/*------------------------------------------------------------------------*/
/* file: DrawableRectangleFactory.h                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for rectangle objects. Automaticaly create a rectangle  */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_RECTANGLE_FACTORY_H_
#define _DRAWABLE_RECTANGLE_FACTORY_H_


#include "../DrawableObjectFactory.h"

namespace sciGraphics
{

class DrawableRectangleFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  DrawableObject * create( void ) ;

} ;

}

#endif /* _DRAWABLE_RECTANGLE_FACTORY_H_  */
