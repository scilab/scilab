/*------------------------------------------------------------------------*/
/* file: DrawableObject.h                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class dedicated for the graphic rendering of graphic handles    */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_OBJECT_H_
#define _DRAWABLE_OBJECT_H_

extern "C"
{
#include "ObjectStructure.h"
}

/**
 * Object dedicated to render a sciPointObj.
 */
class DrawableObject
{

public:

  DrawableObject( sciPointObj * drawed ) { m_pDrawed = drawed ; }

  virtual ~DrawableObject( void ) {}

  /**
   * Main function of the class. Render the graphic handle
   */
  virtual void draw( void ) = 0 ;

  /**
   * Common to every object, draw their children
   */
  void drawChildren( void ) ;

  /**
   * Common to every object. Check its visibility.
   */
  bool checkVisibility( void ) ;

  /**
   * In case you want to retrieve the drawed object
   */
  sciPointObj * getDrawedObject( void ) { return m_pDrawed ; }

protected:

  /**
   * The graphic handle to draw
   */
  sciPointObj * m_pDrawed ;

} ;

#endif /* _DRAWABLE_OBJECT_H_  */
