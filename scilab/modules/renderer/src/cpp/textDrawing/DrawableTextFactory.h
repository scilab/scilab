/*------------------------------------------------------------------------*/
/* file: DrawableTextFactory.h                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for text objects. Automaticaly create a text            */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_TEXT_FACTORY_H_
#define _DRAWABLE_TEXT_FACTORY_H_


#include "../DrawableObjectFactory.h"
#include "ConcreteDrawableText.hxx"

namespace sciGraphics
{

class DrawableTextFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  virtual DrawableObject * create( void ) ;

  /**
   * Update the drawer of the drawed object.
   * To be used when an object was deeply modified and to avoid to create it again completely.
   */
  virtual void update( void ) ;

protected:

  /**
   * Create the drawing strategies.
   * @param text Object on which to create the strategies
   */
  void setStrategies(ConcreteDrawableText * text) ;

} ;

}

#endif /* _DRAWABLE_TEXT_FACTORY_H_  */
