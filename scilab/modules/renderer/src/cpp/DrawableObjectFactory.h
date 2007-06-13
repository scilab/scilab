/*------------------------------------------------------------------------*/
/* file: DrawableObjectFactory.h                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for drawable objects. Automaticaly create a drawable    */
/*        object with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_OBJECT_FACTORY_H_
#define _DRAWABLE_OBJECT_FACTORY_H_

extern "C"
{
#include "../../../graphics/includes/ObjectStructure.h"
}
#include "DrawableObject.h"

namespace sciGraphics
{

class DrawableObjectFactory
{

public:

  DrawableObjectFactory( void ) { m_pDrawed = NULL ; }

  virtual ~DrawableObjectFactory( void ) {}
  
  /**
   * Set the graphic handle we want to draw.
   * The factory will create the right kind of drawer and choose the
   * algorithms from drawedObj.
   */
  void setGraphicObj( sciPointObj * drawedObj ) ;

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

  sciPointObj * m_pDrawed ;

private:

  /**
   * Choose the good factory to create the drawer
   */
  DrawableObjectFactory * createRightFactory( void ) ;

} ;

}

#endif /* _DRAWABLE_OBJECT_FACTORY_H_  */
