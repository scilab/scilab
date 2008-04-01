/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the drawing routine for a subwin object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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

  /**
   * Display the subwin by just setting the camera
   * and its children pObj
   */
  void displaySingleObj(sciPointObj * pObj);

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
   * Draw the bounding box and ticks of the subwin object.
   */
  void drawAxesBox(void);

  /**
   * Draw the bounding box and ticks of a subwin object with alredy computed data.
   */
  void showAxesBox(void);

  /**
   * Draw the surrounding cube of the subwin object.
   */
  virtual void drawBox(void) = 0;

  /**
   * Draw the ticks of the subwin object.
   */
  virtual void drawTicks(void) = 0;

  /**
   * Draw the surrounding cube of the subwin object from alraedy computed data.
   */
  virtual void showBox(void) = 0;

  /**
   * Draw the ticks of the subwin from alraedy computed data.
   */
  virtual void showTicks(void) = 0;

  /**
   * draw the subwin by just setting the camera
   * and its children pObj
   */
  virtual void drawSingleObj(sciPointObj * pObj);

  /**
   * show the subwin by just setting the camera
   * and its children pObj
   */
  virtual void showSingleObj(sciPointObj * pObj);

  /**
   * Return the real type of implementation object
   */
  DrawableSubwinBridge * getSubwinImp( void ) ;

  /**
   * Compute real data bounds from the user ones.
   * ie fills Frect.
   */
  virtual void computeRealDataBounds(void) = 0;


  /*-----------------------------------------------------------------------------*/
  /** Camera used to visualize this axes */
  Camera * m_pCamera;
  /*-----------------------------------------------------------------------------*/


} ;

}

#endif /* _DRAWABLE_SUBWIN_H_  */
