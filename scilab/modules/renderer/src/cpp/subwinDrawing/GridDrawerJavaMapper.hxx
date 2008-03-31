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
#include "GridDrawerGL.hxx"


namespace sciGraphics
{

/**
 * Contains mapping of java method used by GridDrawerJoGL 
 */
class GridDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  GridDrawerJavaMapper(void);

  virtual ~GridDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited from DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for GridDrawer
  virtual void drawGrid(void);

  virtual void setGridParameters(int gridColor, float gridThickness);

  virtual void setGridStartPoints(const double startPointsX[],
                                  const double startPointsY[],
                                  const double startPointsZ[],
                                  int nbPoints);

  virtual void setGridMiddlePoints(const double middlePointsX[],
                                   const double middlePointsY[],
                                   const double middlePointsZ[],
                                   int nbPoints);

  virtual void setGridEndPoints(const double endPointsX[],
                                const double endPointsY[],
                                const double endPointsZ[],
                                int nbPoints);
  /*----------------------------------------------------------------------*/

protected:

  org_scilab_modules_renderer_subwinDrawing::GridDrawerGL * m_pJavaObject;


};

}

#endif /* _GRID_DRAWER_JAVA_MAPPER_HXX_ */

