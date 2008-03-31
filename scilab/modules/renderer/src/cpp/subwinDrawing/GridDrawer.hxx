/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class specialized in drawing grid on subwin axes  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _GRID_DRAWER_HXX_ 
#define _GRID_DRAWER_HXX_

#include "../DrawableObjectBridge.h"
#include "DrawableSubwin.h"
#include "ComputeTicksStrategy.hxx"

namespace sciGraphics
{

class GridDrawer : public virtual DrawableObjectBridge
{
public:

  GridDrawer(DrawableSubwin * subwin);

  virtual ~GridDrawer(void);

  /**
   * Draw the grid from the specified computed points
   */
  virtual void drawGrid(void) = 0;

  /**
   * Specify the starting points for grid
   */
  virtual void setGridStartPoints(const double startPointsX[],
                                  const double startPointsY[],
                                  const double startPointsZ[],
                                  int nbPoints) = 0;

  /**
   * Specify the middle point for grid
   */
  virtual void setGridMiddlePoints(const double middlePointsX[],
                                   const double middlePointsY[],
                                   const double middlePointsZ[],
                                   int nbPoints) = 0;

  /**
   * Specify the end point for grid
   */
  virtual void setGridEndPoints(const double endPointsX[],
                                const double endPointsY[],
                                const double endPointsZ[],
                                int nbPoints) = 0;

protected:

};

}

#endif /* _GRID_DRAWER_HXX_ */

