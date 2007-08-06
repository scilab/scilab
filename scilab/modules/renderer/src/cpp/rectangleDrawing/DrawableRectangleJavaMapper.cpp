/*------------------------------------------------------------------------*/
/* file: DrawableRectangleJavaMapper.cpp                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by DrawableRectangle       */
/*------------------------------------------------------------------------*/

#include "DrawableRectangleJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
DrawableRectangleJavaMapper::DrawableRectangleJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_rectangleDrawing::DrawableRectangleGL(getScilabJavaVM());
}
/*------------------------------------------------------------------------------------------*/
DrawableRectangleJavaMapper::~DrawableRectangleJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleJavaMapper::show(void)
{
  m_pJavaObject->show();
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*-----------------------------------------------------------------------------------*/
void DrawableRectangleJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*----------------------------------------------------------------------*/
void DrawableRectangleJavaMapper::clipX(double xMin, double xMax)
{
  m_pJavaObject->clipX(xMin, xMax);
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleJavaMapper::clipY(double yMin, double yMax)
{
  m_pJavaObject->clipY(yMin, yMax);
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleJavaMapper::clipZ(double zMin, double zMax)
{
  m_pJavaObject->clipZ(zMin, zMax);
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleJavaMapper::unClip(void)
{
  m_pJavaObject->unClip();
}
/*------------------------------------------------------------------------------------------*/
}
