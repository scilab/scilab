/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by StandardTextDrawerJoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "StandardTextDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
StandardTextDrawerJavaMapper::StandardTextDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_textDrawing::StandardTextDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
StandardTextDrawerJavaMapper::~StandardTextDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::destroy(int figureIndex)
{
  m_pJavaObject->destroy(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::setTextParameters(int textAlignment, int color, int fontStyle,
                                                     double fontSize, double rotationAngle, bool useFractionalMetrics)
{
  m_pJavaObject->setTextParameters(textAlignment, color, fontStyle, fontSize, rotationAngle, useFractionalMetrics);
}
/*---------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::updateParentFigure(int parentFigureIndex)
{
  m_pJavaObject->updateParentFigure(parentFigureIndex);
}
/*---------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::setBoxDrawingParameters(bool drawBoxLine, bool drawBoxBackground,
			                                   int lineColor, int backgroundColor)
{
  m_pJavaObject->setBoxDrawingParameters(drawBoxLine, drawBoxBackground, lineColor, backgroundColor);
}
/*---------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::setTextContent(char ** text, int nbRow, int nbCol)
{
  m_pJavaObject->setTextContent(text, nbRow * nbCol, nbRow, nbCol);
}
/*---------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::setCenterPosition(double centerX, double centerY, double centerZ)
{
  m_pJavaObject->setCenterPosition(centerX, centerY, centerZ);
}
/*---------------------------------------------------------------------------------*/
double * StandardTextDrawerJavaMapper::drawTextContent(void)
{
  return m_pJavaObject->drawTextContent();
}
/*---------------------------------------------------------------------------------*/
double * StandardTextDrawerJavaMapper::getScreenBoundingBox(double centerPixX,
                                                            double centerPixY,
                                                            double centerPixZ)
{
  return m_pJavaObject->getScreenBoundingBox(centerPixX, centerPixY, centerPixZ);
}
/*---------------------------------------------------------------------------------*/
}
