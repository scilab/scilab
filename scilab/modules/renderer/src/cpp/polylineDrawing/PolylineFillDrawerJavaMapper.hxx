/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of java method used by PolylineFillDrawer 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _POLYLINE_FILL_DRAWER_JAVA_MAPPER_HXX_
#define _POLYLINE_FILL_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "PolylineFillDrawerGL.hxx"

namespace sciGraphics
{

class PolylineFillDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{

public:

  PolylineFillDrawerJavaMapper(void);

  virtual ~PolylineFillDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);

  virtual void setFigureIndex(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for rectangle fill drawer
  virtual void setBackColor(int color);

  virtual void drawPolyline(const double xCoords[],
                            const double yCoords[],
                            const double zCoords[],
                            int nbCoords);
  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_polylineDrawing::PolylineFillDrawerGL * m_pJavaObject;

};

}

#endif /* _POLYLINE_FILL_DRAWER_JAVA_MAPPER_HXX_ */

