/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of java method used by TextContentDrawerJoGL  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _TEXT_CONTENT_DRAWER_JAVA_MAPPER_HXX_
#define _TEXT_CONTENT_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"

namespace sciGraphics
{

class TextContentDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  TextContentDrawerJavaMapper( void ) {}

  virtual ~TextContentDrawerJavaMapper( void ) {}

  virtual void setTextContent(char ** text, int nbRow, int nbCol) = 0;
  virtual void setCenterPosition(double centerX, double centerY, double centerZ) = 0;
  virtual double * drawTextContent(void) = 0;

  virtual double * getScreenBoundingBox(double centerPixX,
                                        double centerPixY,
                                        double centerPixZ) = 0;
  virtual void setTextParameters(int textAlignment, int color, int fontStyle,
                                 double fontSize, double rotationAngle, bool useFractionalMetrics) = 0;

  virtual void updateParentFigure(int parentFigureIndex) = 0;

  virtual void setBoxDrawingParameters(bool drawBoxLine, bool drawBoxBackground,
			               int lineColor, int backgroundColor) = 0;

};

}

#endif /* _TEXT_CONTENT_DRAWER_JAVA_MAPPER_HXX_ */

