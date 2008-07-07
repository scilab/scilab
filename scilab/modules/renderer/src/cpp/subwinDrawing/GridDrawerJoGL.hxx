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
   * Draw the grid
   */
  virtual void drawGrid(const double firstAxisStart[3], const double firstAxisEnd[3],
                        const double secondAxisStart[3], const double secondAxisEnd[3],
                        const double thirdAxisStart[3], const double thirdAxisEnd[3],
                        const double relativeTicksPositions[], int nbTicks);

  /**
   * Specify initialize drawing in setting all useful parameters.
   */
  virtual void initializeDrawing(void);

  /**
   * Specify end drawing in setting all useful parameters.
   */
  virtual void endDrawing(void);

  /**
   * Same as initialize drawing but doesn't set constant parameters
   */
  virtual void initializeShowing(void);

  /**
   * Same as end drawing but doesn't set constant parameters
   */
  virtual void endShowing(void);

  /**
   * Get the object performing mapping with Java class.
   */
  GridDrawerJavaMapper * getGridDrawerJavaMapper(void);

};

}

#endif /* _GRID_DRAWER_JOGL_HXX_ */

