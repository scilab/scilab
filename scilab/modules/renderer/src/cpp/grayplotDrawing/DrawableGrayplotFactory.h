/*------------------------------------------------------------------------*/
/* file: DrawableGrayplotFactory.h                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for grayplot objects. Automaticaly create a grayplot    */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_GRAYPLOT_FACTORY_H_
#define _DRAWABLE_GRAYPLOT_FACTORY_H_


#include "../DrawableObjectFactory.h"
#include "ConcreteDrawableGrayplot.hxx"

namespace sciGraphics
{

class DrawableGrayplotFactory : public DrawableObjectFactory
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
   * Set the startegies used by the grayplot object
   */
  void setStrategies(ConcreteDrawableGrayplot * grayplot);

} ;

}

#endif /* _DRAWABLE_GRAYPLOT_FACTORY_H_  */
