/*------------------------------------------------------------------------*/
/* file: DrawableSubwin.h                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a subwin object        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SUBWIN_H_
#define _DRAWABLE_SUBWIN_H_


#include "../DrawableClippedObject.h"
#include "Camera.h"
#include "DrawableSubwinBridge.h"

namespace sciGraphics
{

class DrawableSubwin : public DrawableClippedObject
{

public:

  DrawableSubwin( sciPointObj * pObj ) ;

  virtual ~DrawableSubwin( void ) ;

  /**
   * Set/Get the current camera
   */
  Camera * getCamera( void ) { return m_pCamera; }

  void setCamera( Camera * cam ) { m_pCamera = cam ; }

protected:

  /**
   * Draw the graphic handle and store it representation in memory
   * for later faster drawing.
   */
  virtual void draw( void ) ;

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual void show( void ) ;

  /**
   * Return the real type of implementation object
   */
  DrawableSubwinBridge * getSubwinImp( void ) ;

  /*--------------------------------------------------------------------------------------*/
  /** Camera used to visualize this axes */
  Camera * m_pCamera;
  /*--------------------------------------------------------------------------------------*/


} ;

}

#endif /* _DRAWABLE_SUBWIN_H_  */
