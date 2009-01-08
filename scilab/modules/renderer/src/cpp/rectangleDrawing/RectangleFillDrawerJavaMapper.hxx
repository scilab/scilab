/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of java method used by RectangleFillDrawer
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _RECTANGLE_FILL_DRAWER_JAVA_MAPPER_HXX_
#define _RECTANGLE_FILL_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "RectangleFillDrawerGL.hxx"

namespace sciGraphics
{

class RectangleFillDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{

public:

  RectangleFillDrawerJavaMapper(void);

  virtual ~RectangleFillDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for rectangle fill drawer
  virtual void setBackColor(int color);

  virtual void drawRectangle(double corner1X, double corner1Y, double corner1Z,
                             double corner2X, double corner2Y, double corner2Z,
                             double corner3X, double corner3Y, double corner3Z,
                             double corner4X, double corner4Y, double corner4Z);
  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_rectangleDrawing::RectangleFillDrawerGL * m_pJavaObject;

};

}

#endif
