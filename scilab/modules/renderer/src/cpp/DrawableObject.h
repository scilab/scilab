/*------------------------------------------------------------------------*/
/* file: DrawableObject.h                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class dedicated for the graphic rendering of graphic handles    */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_OBJECT_H_
#define _DRAWABLE_OBJECT_H_

#include "DrawableObjectImp.h"

extern "C"
{
#include "../../../graphics/includes/ObjectStructure.h"
}

namespace sciGraphics
{

/**
 * Object dedicated to render a sciPointObj.
 */
class DrawableObject
{

public:

  DrawableObject( sciPointObj * drawed ) ;

  virtual ~DrawableObject( void ) ;

  /**
   * Display a handle on the screen.
   */
  virtual void display( void ) ;

  /**
   * In case you want to retrieve the drawed object
   */
  sciPointObj * getDrawedObject( void ) { return m_pDrawed ; }

  /**
   * To specify that the object was modified and need to be redrawn
   */
  void hasChanged( void ) ;

  /**
   * To specify that an object and its children have changed
   */
  void familyHasChanged( void ) ;

  /**
   * Common to every object, draw their children
   */
  void displayChildren( void ) ;

  /**
   * Set the driver dependent implementation of algorithms
   */
  void setDrawableImp( DrawableObjectImp * imp ) { m_pImp = imp ; }

  /**
  * Get the driver dependent implementation of algorithm
  * Each graphic object must have is own
  */
  DrawableObjectImp * getDrawableImp( void ) { return m_pImp ; }

protected:

  /*---------------------------------------------------------------------------------*/
  /**
   * Draw the graphic handle and store it representation in memory
   * for later faster drawing.
   */
  virtual void draw( void ) = 0 ;

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual void show( void ) = 0 ;

  /**
   * Common to every object. Check its visibility.
   */
  bool checkVisibility( void ) ;

  /**
   * Initialize the context for drawing
   */
  virtual void initializeDrawing( void ) ;

  /**
   * Close drawing session and display image on the screen
   */
  virtual void endDrawing( void ) ;


  /*---------------------------------------------------------------------------------*/

  /**
   * The graphic handle to draw
   */
  sciPointObj * m_pDrawed ;

  /**
   * To know if the object needs to be drawn or if show is enough.
   */
  bool m_bNeedRedraw ;

  /**
   *  Contains drivers independent algorithm common to all DrawableObjects
   */
  DrawableObjectImp * m_pImp ;
  /*---------------------------------------------------------------------------------*/

} ;

}

#endif /* _DRAWABLE_OBJECT_H_  */
