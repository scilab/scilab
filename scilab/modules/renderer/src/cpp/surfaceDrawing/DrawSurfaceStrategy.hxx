/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy interface for the drawSurface algorithm 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAW_SURFACE_STRATEGY_HXX_
#define _DRAW_SURFACE_STRATEGY_HXX_

#include "DrawableSurface.h"

namespace sciGraphics
{

class DrawSurfaceStrategy
{

public:

  DrawSurfaceStrategy( DrawableSurface * surface ) { m_pDrawed = surface ; }

  virtual ~DrawSurfaceStrategy( void ) {m_pDrawed = NULL;}


  /**
   * Main algorithm
   */
  virtual void drawSurface( void ) = 0 ;

  /**
   * Call the display list of an object
   */
  virtual void showSurface( void ) = 0;

  /**
   * Draw objects using precomputed data
   * by default use show.
   */
  virtual void redrawSurface( void ) {showSurface();}

protected:

  DrawableSurface * m_pDrawed;

};

}

#endif /* _DRAW_SURFACE_STRATEGY_HXX_ */

