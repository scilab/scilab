/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Parent interface for routine independent algorithms
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_OBJECT_BRIDGE_HXX_
#define _DRAWABLE_OBJECT_BRIDGE_HXX_


extern "C"
{
#include "MALLOC.h"
};

namespace sciGraphics
{

class DrawableObject ;

class DrawableObjectBridge
{
public:

  /**
   * Default constructor
   * Should not take any parameter to ease multiple inheritance
   */
  DrawableObjectBridge( void ) {}

  /**
   * Destructor should not take any parameter
   */
  virtual ~DrawableObjectBridge( void ) {}

  /**
   * Initialize the context for drawing (incl starting recording display list)
   * @param figureIndex index of parentFigure
   */
  virtual void initializeDrawing( void ) = 0 ;

  /**
   * Close drawing session (incl stop recording display list)
   */
  virtual void endDrawing( void ) = 0 ;

  /**
   * Initialize the context for showing (ie displaying recorded object)
   * @param figureIndex index of parentFigure
   */
  virtual void initializeShowing( void ) = 0 ;

  /**
   * Close showing session
   */
  virtual void endShowing( void ) = 0 ;

  /**
   * Translate the object with the record move in camera coordinates
   */
  virtual void translate(const double translation[3]) = 0;

  /**
   * Translate the object with the record move in camera coordinates
   */
  virtual void endTranslate(void) = 0;

  /**
   * Display an object which whose representation has been already saved (ie dl created).
   */
  virtual void show( void ) = 0 ;

  /**
   * To retrieve drawable object properties
   */
  virtual DrawableObject * getDrawer( void ) = 0;

protected:

};
}

#endif /* _DRAWABLE_OBJECT_BRIDGE_HXX_ */
