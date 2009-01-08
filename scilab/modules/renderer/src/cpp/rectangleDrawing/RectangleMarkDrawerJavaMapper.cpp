/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by RectangleMarkDrawer
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "RectangleMarkDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
RectangleMarkDrawerJavaMapper::RectangleMarkDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_rectangleDrawing::RectangleMarkDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
RectangleMarkDrawerJavaMapper::~RectangleMarkDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void RectangleMarkDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void RectangleMarkDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void RectangleMarkDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void RectangleMarkDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void RectangleMarkDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*---------------------------------------------------------------------------------*/
void RectangleMarkDrawerJavaMapper::setMarkParameters(int background, int foreground, int markSizeUnit, int markSize, int markStyleIndex)
{
  m_pJavaObject->setMarkParameters(background, foreground, markSizeUnit, markSize, markStyleIndex);
}
/*---------------------------------------------------------------------------------*/
void RectangleMarkDrawerJavaMapper::drawRectangle(double corner1X, double corner1Y, double corner1Z,
                                                  double corner2X, double corner2Y, double corner2Z,
                                                  double corner3X, double corner3Y, double corner3Z,
                                                  double corner4X, double corner4Y, double corner4Z)
{
  m_pJavaObject->drawRectangle(corner1X, corner1Y, corner1Z,
                               corner2X, corner2Y, corner2Z,
                               corner3X, corner3Y, corner3Z,
                               corner4X, corner4Y, corner4Z) ;
}
/*---------------------------------------------------------------------------------*/
void RectangleMarkDrawerJavaMapper::drawRectangle(void)
{
  m_pJavaObject->drawRectangle();
}
/*---------------------------------------------------------------------------------*/
}
