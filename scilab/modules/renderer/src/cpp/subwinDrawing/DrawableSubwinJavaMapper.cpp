/*------------------------------------------------------------------------*/
/* file: DrawableSubwinJavaMapper.cpp                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by DrawableSubwin          */
/*------------------------------------------------------------------------*/

#include "DrawableSubwinJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
DrawableSubwinJavaMapper::DrawableSubwinJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_subwinDrawing::DrawableSubwinGL(getScilabJavaVM());
}
/*--------------------------------------------------------------------------*/
DrawableSubwinJavaMapper::~DrawableSubwinJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*--------------------------------------------------------------------------*/
void DrawableSubwinJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*--------------------------------------------------------------------------*/
void DrawableSubwinJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*--------------------------------------------------------------------------*/
void DrawableSubwinJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*--------------------------------------------------------------------------*/
void DrawableSubwinJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*--------------------------------------------------------------------------*/
void DrawableSubwinJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
void DrawableSubwinJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*--------------------------------------------------------------------------*/
void DrawableSubwinJavaMapper::drawBox(double xMin, double xMax,
                                       double yMin, double yMax,
                                       double zMin, double zMax)
{
  m_pJavaObject->drawBox(xMin, xMax, yMin, yMax, zMin, zMax);
}
/*--------------------------------------------------------------------------*/
}
