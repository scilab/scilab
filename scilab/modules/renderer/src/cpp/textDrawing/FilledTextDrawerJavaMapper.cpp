/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by FilledTextDrawerJoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "FilledTextDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
FilledTextDrawerJavaMapper::FilledTextDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_textDrawing::FilledTextDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
FilledTextDrawerJavaMapper::~FilledTextDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void FilledTextDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void FilledTextDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void FilledTextDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void FilledTextDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void FilledTextDrawerJavaMapper::destroy(int figureIndex)
{
  m_pJavaObject->destroy(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void FilledTextDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void FilledTextDrawerJavaMapper::setTextParameters(int textAlignment, int color, int fontStyle,
                                                   double rotationAngle, int boxWidth, int boxHeight)
{
  m_pJavaObject->setTextParameters(textAlignment, color, fontStyle, rotationAngle, boxWidth, boxHeight);
}
/*---------------------------------------------------------------------------------*/
void FilledTextDrawerJavaMapper::setTextContent(char ** text, int nbRow, int nbCol)
{
  m_pJavaObject->setTextContent(text, nbRow * nbCol, nbRow, nbCol);
}
/*---------------------------------------------------------------------------------*/
void FilledTextDrawerJavaMapper::setCenterPosition(double centerX, double centerY, double centerZ)
{
  m_pJavaObject->setCenterPosition(centerX, centerY, centerZ);
}
/*---------------------------------------------------------------------------------*/
void FilledTextDrawerJavaMapper::drawTextContent(void)
{
  m_pJavaObject->drawTextContent();
}
/*---------------------------------------------------------------------------------*/
double * FilledTextDrawerJavaMapper::getBoundingRectangle(void)
{
  return m_pJavaObject->getBoundingRectangle();
}
/*---------------------------------------------------------------------------------*/
long * FilledTextDrawerJavaMapper::getScreenBoundingBox(void)
{
  return m_pJavaObject->getScreenBoundingBox();
}
/*---------------------------------------------------------------------------------*/
}
