/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class specialized in drawing grid using JoGL  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _GRID_DRAWER_JOGL_HXX_
#define _GRID_DRAWER_JOGL_HXX_

#include "../DrawableObjectJoGL.h"
#include "GridDrawer.hxx"
#include "GridDrawerJavaMapper.hxx"

namespace sciGraphics
{

class GridDrawerJoGL : public GridDrawer, public DrawableObjectJoGL
{
public:

  GridDrawerJoGL(DrawableSubwin * subwin);

  virtual ~GridDrawerJoGL(void);

protected:

  /**
   * Specify the starting points for grid
   */
  virtual void setGridStartPoints(const double startPointsX[],
                                  const double startPointsY[],
                                  const double startPointsZ[],
                                  int nbPoints);

  /**
   * Specify the middle point for grid
   */
  virtual void setGridMiddlePoints(const double middlePointsX[],
                                   const double middlePointsY[],
                                   const double middlePointsZ[],
                                   int nbPoints);

  /**
   * Specify the end point for grid
   */
  virtual void setGridEndPoints(const double endPointsX[],
                                const double endPointsY[],
                                const double endPointsZ[],
                                int nbPoints);

  /**
   * Get the object performing mapping with Java class.
   */
  GridDrawerJavaMapper * getGridDrawerJavaMapper(void);

};

}

#endif /* _GRID_DRAWER_JOGL_HXX_ */

