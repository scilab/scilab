/*------------------------------------------------------------------------*/
/* file: DrawablePolylineJavaMapper.cpp                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by DrawablePolyline        */
/*------------------------------------------------------------------------*/

#include "DrawablePolylineJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}


namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
DrawablePolylineJavaMapper::DrawablePolylineJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_polylineDrawing::DrawablePolylineGL(getScilabJavaVM());
}
/*------------------------------------------------------------------------------------------*/
DrawablePolylineJavaMapper::~DrawablePolylineJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*------------------------------------------------------------------------------------------*/
void DrawablePolylineJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*------------------------------------------------------------------------------------------*/
void DrawablePolylineJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void DrawablePolylineJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void DrawablePolylineJavaMapper::show(void)
{
  m_pJavaObject->show();
}
/*------------------------------------------------------------------------------------------*/
void DrawablePolylineJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*-----------------------------------------------------------------------------------*/
void DrawablePolylineJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*----------------------------------------------------------------------*/
void DrawablePolylineJavaMapper::clipX(double xMin, double xMax)
{
  m_pJavaObject->clipX(xMin, xMax);
}
/*------------------------------------------------------------------------------------------*/
void DrawablePolylineJavaMapper::clipY(double yMin, double yMax)
{
  m_pJavaObject->clipY(yMin, yMax);
}
/*------------------------------------------------------------------------------------------*/
void DrawablePolylineJavaMapper::clipZ(double zMin, double zMax)
{
  m_pJavaObject->clipZ(zMin, zMax);
}
/*------------------------------------------------------------------------------------------*/
void DrawablePolylineJavaMapper::unClip(void)
{
  m_pJavaObject->unClip();
}
/*------------------------------------------------------------------------------------------*/
}
