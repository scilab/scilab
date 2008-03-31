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
                                                     double fontSize, double rotationAngle)
{
  m_pJavaObject->setTextParameters(textAlignment, color, fontStyle, fontSize, rotationAngle);
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
void StandardTextDrawerJavaMapper::drawTextContent(void)
{
  m_pJavaObject->drawTextContent();
}
/*---------------------------------------------------------------------------------*/
double * StandardTextDrawerJavaMapper::getBoundingRectangle(void)
{
  return m_pJavaObject->getBoundingRectangle();
}
/*---------------------------------------------------------------------------------*/
long * StandardTextDrawerJavaMapper::getScreenBoundingBox(void)
{
  return m_pJavaObject->getScreenBoundingBox();
}
/*---------------------------------------------------------------------------------*/
void StandardTextDrawerJavaMapper::setBoxParameters(bool isLine, bool isFill, int background, int foreground,
                                                  float lineWidth, int lineStyle)
{
  m_pJavaObject->setBoxParameters(isLine, isFill, background, foreground, lineWidth, lineStyle);
}
/*---------------------------------------------------------------------------------*/
}
