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

class DrawablePolylineFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  DrawableObject * create( void ) ;

} ;

#endif /* _DRAWABLE_POLYLINE_FACTORY_H_  */
