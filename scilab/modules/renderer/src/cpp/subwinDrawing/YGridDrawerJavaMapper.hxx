/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing java methods needed by YGridDrawerJoGL  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _Y_GRID_DRAWER_JAVA_MAPPER_HXX_
#define _Y_GRID_DRAWER_JAVA_MAPPER_HXX_

#include "GridDrawerJavaMapper.hxx"
#include "YGridDrawerGL.hxx"

namespace sciGraphics
{

/**
 * Contains mapping of Java method used by XGridDrawerJoGL 
 */
class YGridDrawerJavaMapper : public virtual GridDrawerJavaMapper
{
public:

  YGridDrawerJavaMapper(void);

  virtual ~YGridDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited from DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for TicksDrawer
  virtual void drawGrid(const double gridPositions[], int nbPositions);

  virtual void setGridParameters(int gridColor, float gridThickness);

  virtual void setAxesBounds(double xMin, double xMax,
                             double yMin, double yMax,
                             double zMin, double zMax);
  /*----------------------------------------------------------------------*/

protected:

  org_scilab_modules_renderer_subwinDrawing::YGridDrawerGL * m_pJavaObject;

};

}

#endif /* _Y_GRID_DRAWER_JAVA_MAPPER_HXX_ */

