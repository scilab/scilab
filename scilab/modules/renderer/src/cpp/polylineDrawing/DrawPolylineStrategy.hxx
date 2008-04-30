/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy interface for the drawPolyline algorithm
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAW_POLYLINE_STRATEGY_HXX_
#define _DRAW_POLYLINE_STRATEGY_HXX_

namespace sciGraphics
{

class DrawablePolyline;

class DrawPolylineStrategy
{

public:

  DrawPolylineStrategy( DrawablePolyline * polyline ) { m_pDrawed = polyline ; }

  virtual ~DrawPolylineStrategy( void ) {m_pDrawed = NULL;}


  /**
   * Main algorithm
   */
  virtual void drawPolyline( void ) = 0 ;

  /**
   * Call the display list of an object
   */
  virtual void showPolyline( void ) = 0;

  /**
   * Use precomputed data, by default just call show
   */
  virtual void redrawPolyline( void ) {showPolyline();}

protected:

  DrawablePolyline * m_pDrawed;

};

}

#endif /* _DRAW_POLYLINE_STRATEGY_HXX_ */

