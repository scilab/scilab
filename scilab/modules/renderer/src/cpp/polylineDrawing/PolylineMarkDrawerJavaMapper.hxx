/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of java method used by PolylineMarkDrawer 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _POLYLINE_MARK_DRAWER_JAVA_MAPPER_HXX_
#define _POLYLINE_MARK_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "PolylineMarkDrawerGL.hxx"

namespace sciGraphics
{

class PolylineMarkDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  PolylineMarkDrawerJavaMapper(void);

  virtual ~PolylineMarkDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for polyline line drawer
  virtual void setMarkParameters(int background, int foreground, int markSizeUnit,
                                 int markSize, int markStyleIndex);

  virtual void drawPolyline(const double xCoords[],
                            const double yCoords[],
                            const double zCoords[],
                            int nbCoords);

  virtual void drawPolyline(void);
  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_polylineDrawing::PolylineMarkDrawerGL * m_pJavaObject;


};

}

#endif /* _RECTANGLE_LINE_DRAWER_JAVA_MAPPER_HXX_ */

