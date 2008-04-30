/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy interface for the drawSegs algorithm  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAW_SEGS_STRATEGY_HXX_
#define _DRAW_SEGS_STRATEGY_HXX_

#include "DrawableSegs.h"

namespace sciGraphics
{

class DrawSegsStrategy
{

public:

  DrawSegsStrategy( DrawableSegs * polyline ) { m_pDrawed = polyline ; }

  virtual ~DrawSegsStrategy( void ) {m_pDrawed = NULL;}


  /**
   * Main algorithm, draw the segs from their positions
   */
  virtual void drawSegs(const double xStarts[], const double xEnds[],
                        const double yStarts[], const double yEnds[],
                        const double zStarts[], const double zEnds[],
                        const int colors[], int nbSegment) = 0 ;

  /**
   * Call the display list of an object
   */
  virtual void showSegs( void ) = 0;

  /**
   * Redraw object using precomputed data.
   * By default show segs.
   */
  virtual void redrawSegs( void ) {showSegs();}

protected:

  DrawableSegs * m_pDrawed;

};

}

#endif /* _DRAW_SEGS_STRATEGY_HXX_ */

