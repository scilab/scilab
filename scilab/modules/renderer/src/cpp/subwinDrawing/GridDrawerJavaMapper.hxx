/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing java methods needed by GridDrawerJoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _GRID_DRAWER_JAVA_MAPPER_HXX_
#define _GRID_DRAWER_JAVA_MAPPER_HXX_

#include "../DrawableObjectJavaMapper.hxx"


namespace sciGraphics
{

/**
 * Contains mapping of java method used by GridDrawerJoGL 
 */
class GridDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  GridDrawerJavaMapper(void) : DrawableObjectJavaMapper() {}

  virtual ~GridDrawerJavaMapper(void) {}

  /*----------------------------------------------------------------------*/
  // specific for TicksDrawer
  virtual void drawGrid(const double gridPositions[], int nbPositions) = 0;

  virtual void setGridParameters(int gridColor, float gridThickness) = 0;

  virtual void setAxesBounds(double xMin, double xMax,
                             double yMin, double yMax,
                             double zMin, double zMax) = 0;
  /*----------------------------------------------------------------------*/

protected:


};

}

#endif /* _GRID_DRAWER_JAVA_MAPPER_HXX_ */

