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

#include "CenteredTextDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
CenteredTextDrawerJavaMapper::CenteredTextDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_textDrawing::CenteredTextDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
CenteredTextDrawerJavaMapper::~CenteredTextDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void CenteredTextDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void CenteredTextDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void CenteredTextDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void CenteredTextDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void CenteredTextDrawerJavaMapper::destroy(int figureIndex)
{
  m_pJavaObject->destroy(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void CenteredTextDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void CenteredTextDrawerJavaMapper::setTextParameters(int textAlignment, int color, int fontStyle,
                                                     double fontSize, double rotationAngle,
                                                     int boxWidth, int boxHeight)
{
  m_pJavaObject->setTextParameters(textAlignment, color, fontStyle, fontSize, rotationAngle,
                                   boxWidth, boxHeight);
}
/*---------------------------------------------------------------------------------*/
void CenteredTextDrawerJavaMapper::setTextContent(char ** text, int nbRow, int nbCol)
{
  m_pJavaObject->setTextContent(text, nbRow * nbCol, nbRow, nbCol);
}
/*---------------------------------------------------------------------------------*/
void CenteredTextDrawerJavaMapper::setCenterPosition(double centerX, double centerY, double centerZ)
{
  m_pJavaObject->setCenterPosition(centerX, centerY, centerZ);
}
/*---------------------------------------------------------------------------------*/
void CenteredTextDrawerJavaMapper::drawTextContent(void)
{
  m_pJavaObject->drawTextContent();
}
/*---------------------------------------------------------------------------------*/
double * CenteredTextDrawerJavaMapper::getBoundingRectangle(void)
{
  return m_pJavaObject->getBoundingRectangle();
}
/*---------------------------------------------------------------------------------*/
long * CenteredTextDrawerJavaMapper::getScreenBoundingBox(void)
{
  return m_pJavaObject->getScreenBoundingBox();
}
/*---------------------------------------------------------------------------------*/
}
