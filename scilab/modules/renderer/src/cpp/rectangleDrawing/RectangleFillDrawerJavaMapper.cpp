/*------------------------------------------------------------------------*/
/* file: RectangleFillDrawerJavaMapper.cpp                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of Java method used by RectangleFillDrawer     */
/*------------------------------------------------------------------------*/

#include "RectangleFillDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}


namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
RectangleFillDrawerJavaMapper::RectangleFillDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_rectangleDrawing::RectangleFillDrawerGL(getScilabJavaVM());
}
/*------------------------------------------------------------------------------------------*/
RectangleFillDrawerJavaMapper::~RectangleFillDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*------------------------------------------------------------------------------------------*/
void RectangleFillDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*------------------------------------------------------------------------------------------*/
void RectangleFillDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void RectangleFillDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void RectangleFillDrawerJavaMapper::show(void)
{
  m_pJavaObject->show();
}
/*------------------------------------------------------------------------------------------*/
void RectangleFillDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*-----------------------------------------------------------------------------------*/
void RectangleFillDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void RectangleFillDrawerJavaMapper::setBackColor(int color)
{
  m_pJavaObject->setBackColor(color);
}
/*------------------------------------------------------------------------------------------*/
void RectangleFillDrawerJavaMapper::drawRectangle(double corner1X, double corner1Y, double corner1Z,
                                                  double corner2X, double corner2Y, double corner2Z,
                                                  double corner3X, double corner3Y, double corner3Z,
                                                  double corner4X, double corner4Y, double corner4Z)
{
  m_pJavaObject->drawRectangle(corner1X, corner1Y, corner1Z,
                               corner2X, corner2Y, corner2Z,
                               corner3X, corner3Y, corner3Z,
                               corner4X, corner4Y, corner4Z) ;
}
/*------------------------------------------------------------------------------------------*/

}
