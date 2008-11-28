/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of Java method used by CenteredTextDrawerJoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _CENTERED_TEXT_DRAWER_JAVA_MAPPER_HXX_
#define _CENTERED_TEXT_DRAWER_JAVA_MAPPER_HXX_

#include "TextContentDrawerJavaMapper.hxx"
#include "CenteredTextDrawerGL.hxx"

namespace sciGraphics
{

class CenteredTextDrawerJavaMapper : public virtual TextContentDrawerJavaMapper
{

public:

  CenteredTextDrawerJavaMapper(void);

  virtual ~CenteredTextDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);
  /*----------------------------------------------------------------------*/
  // Inherited from TextContentDrawerJavaMapper
  virtual void setTextContent(char ** text, int nbRow, int nbCol);
  virtual void setCenterPosition(double centerX, double centerY, double centerZ);
  virtual double * drawTextContent(void);

  virtual double * getScreenBoundingBox(double centerPixX,
                                        double centerPixY,
                                        double centerPixZ);

  virtual void setTextParameters(int textAlignment, int color, int fontStyle,
                                 double fontSize, double rotationAngle, bool useFractionalMetrics);

  virtual void updateParentFigure(int parentFigureIndex);

  virtual void setBoxDrawingParameters(bool drawBoxLine, bool drawBoxBackground,
			               int lineColor, int backgroundColor);
  /*----------------------------------------------------------------------*/
  // specific for centered text drawer
  virtual void setFilledBoxSize(double boxWidth, double boxHeight);
  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_textDrawing::CenteredTextDrawerGL * m_pJavaObject;


};

}

#endif /* _CENTERED_TEXT_DRAWER_JAVA_MAPPER_HXX_ */

