/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Contains mapping of java method used by RectangleLineDrawer 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "RectangleLineDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
RectangleLineDrawerJavaMapper::RectangleLineDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_rectangleDrawing::RectangleLineDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
RectangleLineDrawerJavaMapper::~RectangleLineDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void RectangleLineDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void RectangleLineDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void RectangleLineDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void RectangleLineDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void RectangleLineDrawerJavaMapper::destroy(int parentFigureIndex)
{
 m_pJavaObject->destroy(parentFigureIndex); 
}
/*---------------------------------------------------------------------------------*/
void RectangleLineDrawerJavaMapper::setLineParameters(int lineColor, float thickness, int lineStyle)
{
  m_pJavaObject->setLineParameters(lineColor, thickness, lineStyle);
}
/*---------------------------------------------------------------------------------*/
void RectangleLineDrawerJavaMapper::drawRectangle(double corner1X, double corner1Y, double corner1Z,
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

}
