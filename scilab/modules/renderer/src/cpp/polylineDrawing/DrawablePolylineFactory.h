/*------------------------------------------------------------------------*/
/* file: DrawablePolylineFactory.h                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for polyline objects. Automaticaly create a polyline    */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_POLYLINE_FACTORY_H_
#define _DRAWABLE_POLYLINE_FACTORY_H_


#include "../DrawableObjectFactory.h"
#include "DrawablePolyline.h"

namespace sciGraphics
{

class DrawablePolylineFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  DrawableObject * create( void ) ;

  /**
   * Update the drawer of the drawed object.
   * To be used when an object was deeply modified and to avoid to create it again completely.
   */
  virtual void update( void ) ;

protected:

  /**
   * Create the drawing strategies.
   * @param arc Object on which to create the strategies
   */
  void setStrategies( DrawablePolyline * polyline ) ;

} ;

}

#endif /* _DRAWABLE_POLYLINE_FACTORY_H_  */
