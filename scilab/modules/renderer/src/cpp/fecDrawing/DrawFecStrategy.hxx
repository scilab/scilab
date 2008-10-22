/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy for drawing a fec object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _FEC_DRAWING_STRATEGY_HXX_
#define _FEC_DRAWING_STRATEGY_HXX_

#include "DrawableFec.h"

namespace sciGraphics
{

class DrawFecStrategy
{
public:

  DrawFecStrategy(void) {}

  virtual ~DrawFecStrategy(void) {}

  /**
   * Draw the fec using this strategy
   * @param xCoords X coordinates of nodes
   * @param yCoords Y coordinates of nodes
   * @param values value for each node
   * @param nbNodes number of nodes (size of xCoords and yCoords).
   * @param firstPoints node indexes of triangles first point
   * @param seconfPoints node indexes of triangles second point
   * @param seconfPoints node indexes of triangles third point
   * @param nbTriangles number of triangle (size of first, second and thirdPoints).
   */
  virtual void drawFec(const double xCoords[], const double yCoords[],
                       const double values[], int nbNodes,
                       const int firstPoints[], const int secondPoints[],
                       const int thirdPoints[], int nbTriangles) = 0;

  /**
   * Show the fec using this strategy
   */
  virtual void showFec(void) = 0;

protected:

  virtual DrawableFec * getFecDrawer(void) = 0;

};

}

#endif /* _FEC_DRAWING_STRATEGY_HXX_ */

