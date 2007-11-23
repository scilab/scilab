/*------------------------------------------------------------------------*/
/* file: RectangleLineDrawerJavaMapper.cpp                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by RectangleLineDrawer     */
/*------------------------------------------------------------------------*/

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
/*--------------------------------------------------------------------------*/
void RectangleLineDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
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
