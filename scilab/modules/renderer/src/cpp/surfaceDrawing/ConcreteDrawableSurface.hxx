/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing implementation of driver independent algorithms
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _CONCRETE_DRAWABLE_SURFACE_HXX_
#define _CONCRETE_DRAWABLE_SURFACE_HXX_

#include <list>

#include "DrawableSurface.h"
#include "DrawSurfaceStrategy.hxx"


namespace sciGraphics
{

class ConcreteDrawableSurface : public DrawableSurface
{
public:
  ConcreteDrawableSurface(sciPointObj * pSurface);

  ~ConcreteDrawableSurface(void);

  /**
   * Add a rendering algorithm
   */
  void addDrawingStrategy(DrawSurfaceStrategy * strategy);

  /**
   * Remove all drawing strategies.
   */
  void removeDrawingStrategies(void);

protected:
  /*---------------------------------------------------------------------------------*/
  /**
   * Actually draw the surface on the screen
   */
  virtual void drawSurface(void);

  /**
   * Actually show the surface stored data
   */
  virtual void showSurface(void);

  /**
   * Actually redraw the surface stored data
   */
  virtual void redrawSurface(void);
  /*---------------------------------------------------------------------------------*/
  std::list<DrawSurfaceStrategy *> m_oDrawingStrategies;
  /*---------------------------------------------------------------------------------*/
};

}

#endif

