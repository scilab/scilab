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
#include "ObjectStructure.h"
}
#include "DrawableObject.h"

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

protected:

  sciPointObj * m_pDrawed ;

} ;

#endif /* _DRAWABLE_OBJECT_FACTORY_H_  */
