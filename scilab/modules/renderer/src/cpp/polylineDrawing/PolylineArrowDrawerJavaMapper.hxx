/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of Java methods used by RectangleArrowDrawer 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _POLYLINE_ARROW_DRAWER_JAVA_MAPPER_HXX_
#define _POLYLINE_ARROW_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "PolylineArrowDrawerGL.hxx"

namespace sciGraphics
{

class PolylineArrowDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{

public:

  PolylineArrowDrawerJavaMapper(void);

  virtual ~PolylineArrowDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for polyline arrow drawer
  virtual void setArrowParameters(int arrowColor, double arrowSize);

  virtual void setAxesBounds(double xMin, double xMax,
                             double yMin, double yMax,
                             double zMin, double zMax);

  virtual void drawPolyline(const double xCoords[],
                            const double yCoords[],
                            const double zCoords[],
                            int nbCoords);

  virtual void drawPolyline(void);
  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_polylineDrawing::PolylineArrowDrawerGL * m_pJavaObject;


};

}

#endif /* _POLYLINE_ARROW_DRAWER_JAVA_MAPPER_HXX_ */

