/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of Java method used by PolylineBarDrawerJoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _POLYLINE_BAR_DRAWER_JAVA_MAPPER_HXX_
#define _POLYLINE_BAR_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "PolylineBarDrawerGL.hxx"

namespace sciGraphics
{

class PolylineBarDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  PolylineBarDrawerJavaMapper(void);

  virtual ~PolylineBarDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for polyline line drawer
  virtual void setBarParameters(int background, int foreground, float thickness,
                                int lineStyle);

  virtual void drawPolyline(const double left[],
                            const double right[],
                            const double bottom[],
                            const double top[],
                            const double zCoord[],
                            int nbCoords);
  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_polylineDrawing::PolylineBarDrawerGL * m_pJavaObject;


};

}

#endif /* _POLYLINE_BAR_DRAWER_JAVA_MAPPER_HXX_ */

