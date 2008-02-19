/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of java method used by SurfaceLineDrawerJoGL 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _SURFACE_LINE_DRAWER_JAVA_MAPPER_HXX_
#define _SURFACE_LINE_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "SurfaceLineDrawerGL.hxx"

namespace sciGraphics
{

class SurfaceLineDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  SurfaceLineDrawerJavaMapper(void);

  virtual ~SurfaceLineDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for surface line drawer
  virtual void setLineParameters(int lineColor, float thickness, int lineStyle);

  virtual void setSurfaceType(int typeOfSurface);

  virtual void drawSurface(double xCoords[], int sizeX,
                           double yCoords[], int sizeY,
                           double zCoords[], int sizeZ,
                           int nbFacets);
  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_surfaceDrawing::SurfaceLineDrawerGL * m_pJavaObject;


};

}

#endif /* _SURFACE_LINE_DRAWER_JAVA_MAPPER_HXX_ */

