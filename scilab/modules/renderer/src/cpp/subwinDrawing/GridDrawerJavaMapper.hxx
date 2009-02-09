/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Jean-Baptiste Silvy 
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

#include "DrawableObjectJavaMapper.hxx"
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
  /*----------------------------------------------------------------------*/
  // specific for GridDrawer
  virtual void drawGrid(const double firstAxisStart[3], const double firstAxisEnd[3],
                        const double secondAxisStart[3], const double secondAxisEnd[3],
                        const double thirdAxisStart[3], const double thirdAxisEnd[3],
                        const double relativeTicksPositions[], int nbTicks);

  virtual void setGridParameters(int gridColor, float gridThickness, bool drawFront);
  /*----------------------------------------------------------------------*/


protected:

  /**
   * Giws generated wrapper
   */
  org_scilab_modules_renderer_subwinDrawing::GridDrawerGL * m_pJavaObject;

};

}

#endif /* _GRID_DRAWER_JAVA_MAPPER_HXX_ */

