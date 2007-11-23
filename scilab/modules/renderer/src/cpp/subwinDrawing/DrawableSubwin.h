/*------------------------------------------------------------------------*/
/* file: DrawableSubwin.h                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a subwin object        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SUBWIN_H_
#define _DRAWABLE_SUBWIN_H_


#include "../DrawableObject.h"
#include "Camera.h"
#include "DrawableSubwinBridge.h"

namespace sciGraphics
{

class DrawableSubwin : public DrawableObject
{

public:

  DrawableSubwin( sciPointObj * pObj ) ;

  virtual ~DrawableSubwin( void ) ;

  /**
   * Set/Get the current camera
   */
  Camera * getCamera( void ) { return m_pCamera; }

  /**
   * Set/Get the current camera
   */
  void setCamera( Camera * cam ) ;

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
   * Draw the bounding box of the sunwin object.
   */
  void drawBox(const double bounds[6]);

  /**
   * Return the real type of implementation object
   */
  DrawableSubwinBridge * getSubwinImp( void ) ;

  /*-----------------------------------------------------------------------------*/
  /** Camera used to visualize this axes */
  Camera * m_pCamera;
  /*-----------------------------------------------------------------------------*/


} ;

}

#endif /* _DRAWABLE_SUBWIN_H_  */
